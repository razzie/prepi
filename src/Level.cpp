#include <iostream>
#include <fstream>
#include <exception>
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

Level::Level(Globals* globals, std::string tileset, std::string url)
 : m_globals(globals)
 , m_tileset(new TileSet(globals, tileset))
 , m_offset(0,0)
 , m_unit(32)
 , m_bg(new Background(this))
 , m_player(nullptr)
{
    std::fstream file(url);
    Parser p(file);

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
            Element::Type type = p.getArg<Element::Type>();

            switch(type)
            {
                case Element::Type::GROUND:
                    new GroundElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Element::Visibility, Motion>());
                    break;

                case Element::Type::ENEMY:
                    new EnemyElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Element::Visibility, Motion, unsigned>());
                    break;

                case Element::Type::REWARD:
                    new RewardElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned>());
                    break;

                case Element::Type::PLAYER:
                    m_player = new PlayerElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df>());
                    break;

                case Element::Type::FINISH:
                    new FinishElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df>());
                    break;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << "Failed to add element to level (" << e.what() << ")" << std::endl;
        }
    }
}

Level::~Level()
{

}

Globals* Level::getGlobals()
{
    return m_globals;
}

TileSet* Level::getTileSet()
{
    return m_tileset;
}

void Level::addElement(Element* element)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_elements.insert(element);
}

void Level::removeElement(Element* element)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_elements.erase(element);
}

Background* Level::getBackground()
{
    return m_bg;
}

PlayerElement* Level::getPlayerElement()
{
    return m_player;
}

void Level::setDimension(core::dimension2du dim)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_dimension = dim;
    //m_unit = (m_columns*m_unit) / m_columns;
}

core::dimension2du Level::getDimension() const
{
    return m_dimension;
}

void Level::setUnitSize(unsigned unit)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
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
    tthread::lock_guard<tthread::mutex> guard(m_mutex);

    core::dimension2du screenSize = m_globals->driver->getScreenSize();
    core::dimension2du levelSize = {m_dimension.Width * m_unit, m_dimension.Height * m_unit};

    if (m_player)
    {
        core::rect<s32> plBox = m_player->getBoundingBox();
        plBox += core::position2di(m_player->getPosition().X * m_unit, m_player->getPosition().Y * m_unit);
        plBox -= {(s32)screenSize.Width / 2, (s32)screenSize.Height / 2};
        m_offset.X = plBox.UpperLeftCorner.X + (plBox.getWidth() / 2);
        m_offset.Y = plBox.UpperLeftCorner.Y + (plBox.getHeight() / 2);
    }

    if (levelSize.Width < screenSize.Width)
    {
        int delta = (screenSize.Width - levelSize.Width) / 2;
        m_offset.X = -delta;
    }
    else
    {
        int left_delta = m_offset.X;
        int right_delta = (s32)screenSize.Width - (levelSize.Width - m_offset.X);
        if (ABS(left_delta) < ABS(right_delta)) m_offset.X = 0;
        else m_offset.X = levelSize.Width - screenSize.Width;
    }

    if (levelSize.Height < screenSize.Height)
    {
        int delta = (screenSize.Height - levelSize.Height) / 2;
        m_offset.Y = -delta;
    }
    else
    {
        int top_delta = m_offset.Y;
        int bottom_delta = (s32)screenSize.Height - (levelSize.Height - m_offset.Y);
        if (ABS(top_delta) < ABS(bottom_delta)) m_offset.Y = 0;
        else m_offset.Y = levelSize.Height - screenSize.Height;
    }

    m_bg->draw();

    for (Element* element : m_elements)
    {
        core::rect<s32> box = element->getBoundingBox();
        box += core::position2di(element->getPosition().X * m_unit, element->getPosition().Y * m_unit);
        box -= m_offset;

        // do not draw if outside of screen
        if (box.isRectCollided( {{0, 0}, screenSize} ))
        {
            element->draw();
        }

        element->update();
    }
}
