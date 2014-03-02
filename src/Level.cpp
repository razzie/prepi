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

using namespace irr;

Level::Level(Globals* globals, std::string tileset, std::string url)
 : m_globals(globals)
 , m_tileset(new TileSet(globals, tileset))
 , m_view({0,0,800,600})
 , m_unit(32)
 , m_bg(new Background(this))
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
                    new PlayerElement(this, p.getArgs<unsigned, irr::core::vector2di, irr::core::vector2df>());
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

unsigned Level::getUnitSize() const
{
    return m_unit;
}

void Level::setView(core::rect<s32> view)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_view = view;
}

core::rect<s32> Level::getView() const
{
    return m_view;
}

void Level::update()
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);

    m_bg->draw();

    for (Element* element : m_elements)
    {
        auto box = element->getBoundingBox();
        box += core::position2di(element->getPosition().X * m_unit, element->getPosition().Y * m_unit);
        box -= m_view.UpperLeftCorner;

        // do not draw if outside of screen
        if (box.isRectCollided(m_view))
        {
            element->draw();
        }

        element->update();
    }
}
