#include <iostream>
#include <fstream>
#include <exception>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "EventListener.h"
#include "level\TileSet.h"
#include "effects\EffectManager.h"
#include "Parser.h"
#include "Version.h"
#include "level\Level.h"
#include "level\Background.h"
#include "elements\Element.h"
#include "elements\GroundElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"
#include "elements\PlayerElement.h"
#include "elements\FinishElement.h"

#define ABS(a) (unsigned)(((a) < 0) ? -(a) : (a))

using namespace irr;

static const Version prepiVersion {1, 5};

static float gravity = 5.f;

Level::Level(Globals* globals, std::string tileset)
 : m_globals(globals)
 , m_tileset(new TileSet(globals, tileset))
 , m_physics(new b2World( {0.f, gravity} ))
 , m_effectMgr(new EffectManager(this))
 , m_offset(0, 0)
 , m_unit(96.f)
 , m_origUnit(96)
 , m_bg(new Background(this))
 , m_player(nullptr)
 , m_rewardSum(0)
 , m_camMovement(0.f, 0.f)
 , m_debug(false)
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

void Level::reloadTileSet()
{
    clearLevel();
    m_tileset->reload();
    reloadLevel();
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

    const float speed = (float)elapsedMs / 1000.f;
    const float startingUnit = m_unit;
    float desiredUnit = m_origUnit;
    const core::dimension2du screenSize = m_globals->driver->getScreenSize();
    core::vector2di offset;

    // let the camera follow the player
    if (m_player != nullptr)
    {
        EventListener* l = m_globals->eventListener;
        core::vector2df playerPos = m_player->getPosition() + m_player->getBoundingBox().getCenter();
        core::vector2df playerMovement = m_player->getMovement();

        offset.X = (playerPos.X * m_unit) - (screenSize.Width / 2);
        offset.Y = (playerPos.Y * m_unit) - (screenSize.Height / 2);

        if (l->isRight() || playerMovement.X > 0.5f)
            offset.X += screenSize.Width / 4;
        else if (l->isLeft() || playerMovement.X < -0.5f)
            offset.X -= screenSize.Width / 4;
        if (l->isDown() || playerMovement.Y > 0.5f)
            offset.Y += screenSize.Height / 2;
        else if (l->isUp() || playerMovement.Y < -0.5f)
            offset.Y -= screenSize.Height / 4;

        //if (ABS(playerMovement.X) > 5.f || ABS(playerMovement.Y) > 5.f)
        if (playerMovement.Y > 5.f)
            desiredUnit -= 48.f;
    }

    // dynamic change of unit size
    if (m_unit < desiredUnit - 0.2f)
        m_unit += (speed * m_unit) / 2.f;
    else if (m_unit > desiredUnit + 0.2f)
        m_unit -= (speed * m_unit) / 2.f;

    // resize level to fit screen if too small
    if (m_dimension.Width * m_unit < screenSize.Width)
        m_unit = ((float)screenSize.Width / (float)m_dimension.Width) + 1.f;
    if (m_dimension.Height * m_unit < screenSize.Height)
        m_unit = ((float)screenSize.Height / (float)m_dimension.Height) + 1.f;

    // change view according to the change in unit size (focus point: screen center)
    if (ABS(startingUnit - m_unit) > 0.2f)
    {
        core::vector2di screenCenter(screenSize.Width / 2, screenSize.Height / 2);
        m_offset = (m_offset + screenCenter) * m_unit / startingUnit - screenCenter;
        offset = (offset + screenCenter) * m_unit / startingUnit - screenCenter;
        m_camMovement *= m_unit / startingUnit;
    }

    // speed up camera in the direction of the new target position
    const float camSpeed = speed / 8.f;
    m_camMovement.X += camSpeed * (offset.X - m_offset.X);
    m_camMovement.Y += camSpeed * (offset.Y - m_offset.Y);

    // 'fast braking' if camera is too fast and left the target position behind
    if (ABS(offset.X - m_offset.X) < screenSize.Width / 8)
        m_camMovement.X /= 1.2f;
    if (ABS(offset.Y - m_offset.Y) < screenSize.Height / 8)
        m_camMovement.Y /= 1.2f;

    // actually moving the camera
    m_offset.X += m_camMovement.X;
    m_offset.Y += m_camMovement.Y;

    core::dimension2du levelSize;
    levelSize.Width = m_dimension.Width * (unsigned)m_unit;
    levelSize.Height = m_dimension.Height * (unsigned)m_unit;

    // align the level if an edge is out of the screen
    if (m_offset.X < 0)
        m_offset.X = 0;
    else if (m_offset.X > (s32)(levelSize.Width - screenSize.Width))
        m_offset.X = (s32)(levelSize.Width - screenSize.Width);

    // align the level if an edge is out of the screen
    if (m_offset.Y < 0)
        m_offset.Y = 0;
    else if (m_offset.Y > (s32)(levelSize.Height - screenSize.Height))
        m_offset.Y = (s32)(levelSize.Height - screenSize.Height);
}

bool Level::isElementOnScreen(Element* element)
{
    core::dimension2du screenSize = m_globals->driver->getScreenSize();

    core::rectf box = element->getBoundingBox();// + element->getPosition();
    core::recti pixelBox(getScreenPosition(element),
        core::vector2di((unsigned)m_unit * box.getWidth(), (unsigned)m_unit * box.getHeight()));

    return pixelBox.isRectCollided( {{0, 0}, screenSize} );
}

void Level::setDimension(core::dimension2du dim)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_dimension = dim;
}

core::dimension2du Level::getDimension() const
{
    return m_dimension;
}

void Level::setUnitSize(unsigned unit)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_origUnit = unit;
}

unsigned Level::getUnitSize() const
{
    return (unsigned)m_unit;
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
    core::vector2di calcPos = {(s32)(pos.X * (unsigned)m_unit), (s32)(pos.Y * (unsigned)m_unit)};
    calcPos -= m_offset;

    return calcPos;
}

core::vector2df Level::getRealPosition(irr::core::vector2di screenPos) const
{
    screenPos += m_offset;
    return core::vector2df((float)screenPos.X / (float)m_unit, (float)screenPos.Y / (float)m_unit);
}

void Level::switchDebugMode()
{
    m_debug = !m_debug;
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
            if (m_debug) element->getShape().draw(this, element->getPosition());
        }
    }

    // rendering effects
    m_effectMgr->update(elapsedMs);

    // remove elements queued for deletion
    processDeletionQueue();
}
