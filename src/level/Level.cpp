#include <iostream>
#include <fstream>
#include <exception>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "level\TileSet.h"
#include "effects\EffectManager.h"
#include "Parser.h"
#include "level\Level.h"
#include "level\Background.h"
#include "elements\Element.h"
#include "elements\GroundElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"
#include "elements\PlayerElement.h"
#include "elements\FinishElement.h"

#define ABS(a) (((a) < 0) ? -(a) : (a))

using namespace irr;

struct Version
{
    unsigned major;
    unsigned minor;

    inline friend std::istream& operator>> (std::istream& is, Version& ver)
    {
        is.ignore(); // skipping 'v'
        is >> ver.major;
        is.ignore(); // skipping '.'
        is >> ver.minor;
        return is;
    }

    inline friend std::ostream& operator<< (std::ostream& os, const Version& ver)
    {
        os << "v" << ver.major << "." << ver.minor;
        return os;
    }

    bool operator== (const Version& ver)
    {
        return (major == ver.major && minor == ver.minor);
    }

    bool operator!= (const Version& ver)
    {
        return !(*this == ver);
    }
};

static const Version prepiVersion {1, 5};

float gravity = 5.f; // should not be static!

Level::Level(Globals* globals, std::string tileset)
 : m_globals(globals)
 , m_tileset(new TileSet(globals, tileset))
 , m_physics(new b2World( {0.f, gravity} ))
 , m_effectMgr(new EffectManager(this))
 , m_offset(0, 0)
 , m_unit(64)
 , m_bg(new Background(this))
 , m_player(nullptr)
 , m_rewardSum(0)
 , m_camMovement(0, 0)
{
}

Level::~Level()
{
    clearLevel();
    delete m_physics;
    delete m_bg;
    delete m_tileset;
}

Globals* Level::getGlobals()
{
    return m_globals;
}

TileSet* Level::getTileSet()
{
    return m_tileset;
}

b2World* Level::getPhysics()
{
    return m_physics;
}

EffectManager* Level::getEffectManager()
{
    return m_effectMgr;
}

void Level::loadLevel(std::string file)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    clearLevel();

    std::fstream f(file);
    Parser p(f, ';');

    try
    {
        Version ver = p.getArg<Version>();

        if (ver != prepiVersion)
        {
            std::cout << "Mismatching version (prepi: " << prepiVersion << ", file: " << ver << ")" << std::endl;
            return;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Failed to read version info (" << e.what() << ")" << std::endl;
    }

    try
    {
        // background
        unsigned bgId, bgDrawM, columns, rows;
        std::tie(bgId, bgDrawM, columns, rows) = p.getArgs<unsigned, unsigned, unsigned, unsigned>();

        m_bg->setId(bgId);
        m_bg->setDrawingMethod(static_cast<Background::DrawingMethod>(bgDrawM));
        setDimension({columns, rows});
    }
    catch (const std::exception& e)
    {
        std::cout << "Failed to set background (" << e.what() << ")" << std::endl;
    }

    // elements
    unsigned line = 0;
    while(p.nextLine())
    {
        if (f.peek() == '#') continue; // skip comments

        try
        {
            ++line;
            CreateElement(this, f);
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to add element " << line << " to level (" << e.what() << ")" << std::endl;
        }
    }

    m_levelFile = file;
}

void Level::reloadLevel()
{
    loadLevel(m_levelFile);
}

void Level::clearLevel()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    m_timer.reset();
    m_rewardSum = 0;
    m_player = nullptr;
    m_bg->setId(0);

    for (Element* element : m_elemInsertionQueue) delete element;
    m_elemInsertionQueue.clear();

    for (Element* element : m_elemDeletionQueue) delete element;
    m_elemDeletionQueue.clear();

    for (Element* element : m_elements) delete element;
    m_elements.clear();

    m_effectMgr->reset();
}

void Level::addElement(Element* element)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_elemInsertionQueue.push_back(element);
}

void Level::removeElement(Element* element)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    // an element shouldn't be inserted twice
    for (Element* listElem : m_elemDeletionQueue)
    {
        if (listElem == element) return;
    }

    m_elemDeletionQueue.push_back(element);
}

PlayerElement* Level::findPlayer()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    for (Element* element : m_elements)
    {
        if (element->getType() == Element::Type::PLAYER)
            return static_cast<PlayerElement*>(element);
    }

    return nullptr;
}

Background* Level::getBackground()
{
    return m_bg;
}

PlayerElement* Level::getPlayerElement()
{
    return m_player;
}

unsigned Level::getRewardSum() const
{
    return m_rewardSum;
}

void Level::processInsertionQueue()
{
    for (Element* element : m_elemInsertionQueue)
    {
        m_elements.push_back(element);

        if (element->getType() == Element::Type::PLAYER)
            m_player = static_cast<PlayerElement*>(element);

        if (element->getType() == Element::Type::REWARD)
            m_rewardSum += (static_cast<RewardElement*>(element))->getValue();
    }
    m_elemInsertionQueue.clear();
}

void Level::processDeletionQueue()
{
    for (Element* element : m_elemDeletionQueue)
    {
        // a nice inner loop to find the actual elements to delete
        for (auto it = m_elements.begin(); it != m_elements.end(); ++it)
        {
            if (*it == element)
            {
                if (*it == m_player) m_player = findPlayer();

                delete *it;
                m_elements.erase(it);
                break;
            }
        }
    }
    m_elemDeletionQueue.clear();
}

void Level::updateView(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    core::dimension2du screenSize = m_globals->driver->getScreenSize();
    core::dimension2du levelSize = {m_dimension.Width * m_unit, m_dimension.Height * m_unit};
    core::vector2di offset;

    // if level width is small enough, positioning it to center
    if (levelSize.Width < screenSize.Width)
    {
        int delta = (screenSize.Width - levelSize.Width) / 2;
        offset.X = -delta;
    }

    // if level height is small enough, positioning it to center
    if (levelSize.Height < screenSize.Height)
    {
        int delta = (screenSize.Height - levelSize.Height) / 2;
        offset.Y = -delta;
    }

    if (m_player)
    {
        // calculating player's position
        core::rectf box = m_player->getBoundingBox() + m_player->getPosition();
        core::recti plBox( (s32)(box.UpperLeftCorner.X * m_unit),  (s32)(box.UpperLeftCorner.Y * m_unit),
                           (s32)(box.LowerRightCorner.X * m_unit), (s32)(box.LowerRightCorner.Y * m_unit) );
        plBox -= {(s32)screenSize.Width / 2, (s32)screenSize.Height / 2};

        if (levelSize.Width >= screenSize.Width)
        {
            // moving player to the horizontal center of the screen
            offset.X = plBox.UpperLeftCorner.X + (plBox.getWidth() / 2);
        }

        if (levelSize.Height >= screenSize.Height)
        {
            // moving player to the vertical center of the screen
            offset.Y = plBox.UpperLeftCorner.Y + (plBox.getHeight() / 2);
        }
    }

    // speed up camera in the direction of the new target position
    const float camSpeed = (float)elapsedMs / 2000.f;
    m_camMovement.X += camSpeed * (offset.X - m_offset.X) + 1;
    m_camMovement.Y += camSpeed * (offset.Y - m_offset.Y) + 1;

    // 'fast braking' if camera is too fast and left the target position behind
    if ((m_camMovement.X > 0 && m_offset.X > offset.X) ||
        (m_camMovement.X < 0 && m_offset.X < offset.X))
    {
        m_camMovement.X -= (m_camMovement.X / 8);
    }
    if ((m_camMovement.Y > 0 && m_offset.Y > offset.Y) ||
        (m_camMovement.Y < 0 && m_offset.Y < offset.Y))
    {
        m_camMovement.Y -= (m_camMovement.Y / 8);
    }

    // light braking if the fast braking is not accurate enough
    if (m_camMovement.X > 0) m_camMovement.X -= 1;
    else if (m_camMovement.X < 0) m_camMovement.X += 1;
    if (m_camMovement.Y > 0) m_camMovement.Y -= 1;
    else if (m_camMovement.Y < 0) m_camMovement.Y += 1;

    // actually moving the camera
    m_offset += m_camMovement;

    // align the level if an edge is out of the screen
    if (levelSize.Width >= screenSize.Width)
    {
        if (m_offset.X < 0)
        {
            m_offset.X = 0;
            m_camMovement.X = 0;
        }
        else if (m_offset.X > (s32)(levelSize.Width - screenSize.Width))
        {
            m_offset.X = (s32)(levelSize.Width - screenSize.Width);
            m_camMovement.X = 0;
        }
    }

    // align the level if an edge is out of the screen
    if (levelSize.Height >= screenSize.Height)
    {
        if (m_offset.Y < 0)
        {
            m_offset.Y = 0;
            m_camMovement.Y = 0;
        }
        else if (m_offset.Y > (s32)(s32)(levelSize.Height - screenSize.Height))
        {
            m_offset.Y = (s32)(levelSize.Height - screenSize.Height);
            m_camMovement.Y = 0;
        }
    }
}

bool Level::isElementOnScreen(Element* element)
{
    core::dimension2du screenSize = m_globals->driver->getScreenSize();

    core::rectf box = element->getBoundingBox();// + element->getPosition();
    core::recti pixelBox(getScreenPosition(element), core::vector2di(m_unit * box.getWidth(), m_unit * box.getHeight()));

    return pixelBox.isRectCollided( {{0, 0}, screenSize} );
}

void Level::setDimension(core::dimension2du dim)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_dimension = dim;
    //m_unit = (m_columns*m_unit) / m_columns;
}

core::dimension2du Level::getDimension() const
{
    return m_dimension;
}

void Level::setUnitSize(unsigned unit)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_unit = unit;
}

unsigned Level::getUnitSize() const
{
    return m_unit;
}

core::vector2di Level::getViewOffset() const
{
    return m_offset;
}

core::rect<s32> Level::getView() const
{
    return {m_offset, m_globals->driver->getScreenSize()};
}

core::vector2di Level::getScreenPosition(Element* element) const
{
    if (element)
    {
        return getScreenPosition(element->getPosition());
    }
    else
    {
        return core::vector2di(0, 0);
    }
}

core::vector2di Level::getScreenPosition(core::vector2df pos) const
{
    core::vector2di calcPos = {(s32)(pos.X * m_unit), (s32)(pos.Y * m_unit)};
    calcPos -= m_offset;

    return calcPos;
}

core::vector2df Level::getRealPosition(irr::core::vector2di screenPos) const
{
    screenPos += m_offset;
    return core::vector2df((float)screenPos.X / (float)m_unit, (float)screenPos.Y / (float)m_unit);
}

void Level::update()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    // getting elapsed time since the last update
    unsigned elapsedMs = m_timer.getElapsed();

    // adding elements from insertion queue
    processInsertionQueue();

    // updating physics
    static const int32 velocityIterations = 8;
    static const int32 positionIterations = 3;
    m_physics->Step((float)elapsedMs/250, velocityIterations, positionIterations);

    // updating view
    updateView(elapsedMs);

    // drawing background
    m_bg->draw();

    // updating and drawing each element
    for (Element* element : m_elements)
    {
        // if the element is disabled, we skip it
        if (!element->isEnabled())
        {
            continue;
        }

        // first update the elements (position sync, player moving, etc)
        element->update(elapsedMs);

        // do not draw if outside of screen
        if (isElementOnScreen(element))
        {
            element->draw();
            //element->getShape().draw(this, element->getPosition());
        }
    }

    // rendering effects
    m_effectMgr->update(elapsedMs);

    // remove elements queued for deletion
    processDeletionQueue();
}
