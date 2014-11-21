#include <iostream>
#include <fstream>
#include <exception>
#include <cmath>
#include "Box2D\Box2D.h"
#include "Globals.h"
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

#define AUTO_ZOOM 0
#define MAX_PLAYER_MOVE_TIME 2000

using namespace irr;

static double magicFunction(float x)
{
    if (x < 0.f)
        return 0.f;
    else if (x > 1.f)
        return 1.f;
    else if (x < 0.5f)
        return std::pow(x * 2.f, 3) / 2.f;
    else if (x > 0.f)
        return -((std::pow((1.f - x) * 2.f, 3) / 2.f) - 1.f);

    return 0.5f;
}

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
 , m_lastPlayerPos(0.f, 0.f)
 , m_playerMovementTime(0)
 , m_cameraOffset(0.f, 0.f)
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

unsigned Level::getMinimalUnitSize() const
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    const core::dimension2du screenSize = m_globals->driver->getScreenSize();
    unsigned minUnitX = m_unit, minUnitY = m_unit;

    minUnitX = (unsigned)((float)screenSize.Width / (float)m_dimension.Width) + 1;
    minUnitY = (unsigned)((float)screenSize.Height / (float)m_dimension.Height) + 1;

    return (minUnitX > minUnitY) ? minUnitX : minUnitY;
}

void Level::updateView(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    const core::dimension2du screenSize = m_globals->driver->getScreenSize();
    const unsigned minUnit = getMinimalUnitSize();
    const unsigned origUnit = m_unit;

    if (m_player == nullptr)
    {
        core::dimension2du levelSize;

        m_unit = minUnit;
        levelSize.Width = m_dimension.Width * (unsigned)m_unit;
        levelSize.Height = m_dimension.Height * (unsigned)m_unit;
        m_offset.X = (screenSize.Width - levelSize.Width) / 2;
        m_offset.Y = (screenSize.Height - levelSize.Height) / 2;
    }
    else
    {
        const core::vector2df playerPos = m_player->getPosition() + m_player->getBoundingBox().getCenter();
        const core::vector2di playerScreenPos(playerPos.X * (unsigned)m_unit, playerPos.Y * (unsigned)m_unit);
        const core::vector2df playerMovement = playerPos - m_lastPlayerPos;
        const bool playerMoving = (playerMovement.getLength() > 0.01f);
        const double rate = magicFunction((float)m_playerMovementTime / MAX_PLAYER_MOVE_TIME);
        const float camOffset = rate * (screenSize.Width / 4);
        core::dimension2du levelSize;
        core::vector2di offset;

        m_lastPlayerPos = playerPos;
        m_unit = m_origUnit + 0.5f;
        offset.X = playerScreenPos.X - (screenSize.Width / 2);
        offset.Y = playerScreenPos.Y - (screenSize.Height / 2);
        m_cameraOffset -= m_cameraOffset * (float)elapsedMs / 1000.f;

        if (playerMoving)
        {
            core::vector2df oldCameraOffset = m_cameraOffset;
            m_cameraOffset.X += (playerMovement.X / 2.f) * elapsedMs;
            m_cameraOffset.Y += (playerMovement.Y / 16.f) * elapsedMs;

            if (oldCameraOffset.getAngleWith(m_cameraOffset) > 45.f)
            {
                m_playerMovementTime = 0;
            }

            m_playerMovementTime += elapsedMs;
            if (m_playerMovementTime > MAX_PLAYER_MOVE_TIME)
                m_playerMovementTime = MAX_PLAYER_MOVE_TIME;
        }
        else
        {
            if (elapsedMs > m_playerMovementTime)
                m_playerMovementTime = 0;
            else
                m_playerMovementTime -= elapsedMs;

            if (m_playerMovementTime == 0)
                m_cameraOffset.set(0.f, 0.f);
        }

        core::vector2df camVector = core::vector2df(m_cameraOffset).normalize() * camOffset;
        offset.X += camVector.X;
        offset.Y += camVector.Y;

        if (AUTO_ZOOM) m_unit -= (rate * 16.f);

        if (m_unit < minUnit) m_unit = minUnit;

        // change view according to the change in unit size (focus point: screen center)
        if (origUnit != (unsigned)m_unit)
        {
            core::vector2di screenCenter(screenSize.Width / 2, screenSize.Height / 2);
            m_offset = (m_offset + screenCenter) * (unsigned)m_unit / origUnit - screenCenter;
            offset = offset * (unsigned)m_unit / origUnit;
        }

        // moving camera towards desired position (m_offset -> offset)
        m_offset += ((offset - m_offset) * elapsedMs) / 256;

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
