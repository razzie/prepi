#include <iostream>
#include <fstream>
#include <exception>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "TileSet.h"
#include "Parser.h"
#include "Level.h"
#include "Background.h"
#include "Element.h"
#include "GroundElement.h"
#include "EnemyElement.h"
#include "RewardElement.h"
#include "PlayerElement.h"
#include "FinishElement.h"

#define ABS(a) (((a) < 0) ? -(a) : (a))

using namespace irr;

struct Version
{
    unsigned major;
    unsigned minor;

    inline friend std::istream& operator>> (std::istream& is, Version& ver)
    {
        char unused;
        is >> unused; // skipping 'v'
        is >> ver.major;
        is >> unused; // skipping '.'
        is >> ver.minor;
        return is;
    }

    inline friend std::ostream& operator<< (std::ostream& os, const Version& ver)
    {
        os << ver.major << "." << ver.minor;
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

static const Version prepiVersion {1,0};
static const b2Vec2 gravity(0.0f, 5.f);
static int32 velocityIterations = 8;
static int32 positionIterations = 3;

Level::Level(Globals* globals, std::string tileset, std::string url)
 : m_globals(globals)
 , m_tileset(new TileSet(globals, tileset))
 , m_physics(new b2World(gravity))
 , m_offset(0,0)
 , m_unit(64)
 , m_bg(new Background(this))
 , m_player(nullptr)
 , m_rewardSum(0)
{
    std::fstream file(url);
    Parser p(file);

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
    while(p.nextLine())
    {
        try
        {
            CreateElement(this, file);
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to add element to level (" << e.what() << ")" << std::endl;
        }
    }
}

Level::~Level()
{
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

void Level::addElement(Element* element)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_elemInsertionQueue.push_back(element);
}

void Level::removeElement(Element* element)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
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

void Level::updateView()
{
    core::dimension2du screenSize = m_globals->driver->getScreenSize();
    core::dimension2du levelSize = {m_dimension.Width * m_unit, m_dimension.Height * m_unit};

    // if level width is small enough, positioning it to center
    if (levelSize.Width < screenSize.Width)
    {
        int delta = (screenSize.Width - levelSize.Width) / 2;
        m_offset.X = -delta;
    }

    // if level height is small enough, positioning it to center
    if (levelSize.Height < screenSize.Height)
    {
        int delta = (screenSize.Height - levelSize.Height) / 2;
        m_offset.Y = -delta;
    }

    if (m_player)
    {
        // calculating player's position
        core::rect<s32> plBox = m_player->getBoundingBox();
        plBox += core::position2di(m_player->getPosition().X * m_unit, m_player->getPosition().Y * m_unit);
        plBox -= {(s32)screenSize.Width / 2, (s32)screenSize.Height / 2};

        if (levelSize.Width >= screenSize.Width)
        {
            // moving player to the horizontal center of the screen
            m_offset.X = plBox.UpperLeftCorner.X + (plBox.getWidth() / 2);

            // align the level if an edge is out of the screen
            if (m_offset.X < 0)
                m_offset.X = 0;
            else if (m_offset.X > (s32)(levelSize.Width - screenSize.Width))
                m_offset.X = (s32)(levelSize.Width - screenSize.Width);
        }

        if (levelSize.Height >= screenSize.Height)
        {
            // moving player to the vertical center of the screen
            m_offset.Y = plBox.UpperLeftCorner.Y + (plBox.getHeight() / 2);

            // align the level if an edge is out of the screen
            if (m_offset.Y < 0)
                m_offset.Y = 0;
            else if (m_offset.Y > (s32)(s32)(levelSize.Height - screenSize.Height))
                m_offset.Y = (s32)(levelSize.Height - screenSize.Height);
        }
    }
}

bool Level::isElementOnScreen(Element* element)
{
    core::dimension2du screenSize = m_globals->driver->getScreenSize();

    // calculating translated bounding box
    core::rect<s32> box = element->getBoundingBox();
    box += core::position2di(element->getPosition().X * m_unit, element->getPosition().Y * m_unit);
    box -= m_offset;

    // do not draw if outside of screen
    return box.isRectCollided( {{0, 0}, screenSize} );
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

void Level::update()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    unsigned elapsedTime = m_timer.getElapsed();

    // adding elements from insertion queue
    processInsertionQueue();

    // updating physics
    m_physics->Step((float)elapsedTime/250, velocityIterations, positionIterations);

    // updating view
    updateView();

    // drawing background
    m_bg->draw();

    for (Element* element : m_elements)
    {
        // first update the elements (position sync, player moving, etc)
        element->update();

        // do not draw if outside of screen
        if (isElementOnScreen(element))
        {
            element->draw();
        }
    }

    // remove elements queued for deletion
    processDeletionQueue();
}
