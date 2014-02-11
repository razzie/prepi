#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include "Level.h"
#include "Element.h"
#include "Globals.h"

static std::map<unsigned, irr::io::path> bgs = {
    {1, "../media/tilesets/background/01_Fantasy.jpg"},
    {2, "../media/tilesets/background/02_Magic.jpg"},
    {3, "../media/tilesets/background/03_Clear_sky.png"},
    {4, "../media/tilesets/background/04_Dark.png"}};

Level::Level(Globals* globals, std::string url)
 : m_globals(globals)
 , m_view({0,0,800,600})
 , m_unit(64)
 , m_bg(nullptr)
{
    std::fstream file(url);
    char buffer[1024];

    // background
    file.getline(buffer, sizeof(buffer)-1);
    unsigned bgId, bgDrawM, columns, rows;
    std::sscanf(buffer, "%d,%d,%d,%d", &bgId, &bgDrawM, &columns, &rows);
    setBackground(bgId, static_cast<bgDrawingMethod>(bgDrawM));
    setDimension(columns, rows);

    // elements
    /*while (file.getline(buffer, sizeof(buffer)-1))
    {
        std::cout << buffer;


    }*/
}

Level::~Level()
{

}

Globals* Level::getGlobals()
{
    return m_globals;
}

void Level::addElement(Element* element)
{
    m_elements.insert(element);
}

void Level::removeElement(Element* element)
{
    m_elements.erase(element);
}

void Level::setBackground(unsigned id, bgDrawingMethod drawingMethod)
{
    //if (m_bg) m_bg->drop();
    m_bg = m_globals->driver->getTexture(bgs[id]);

    /*irr::video::ITexture* texture = m_globals->driver->getTexture(bgs[id]);
    m_bg = m_globals->driver->createImage(texture, {0,0}, {800,600});
    texture->drop();*/

    //m_bgId = id;
    m_bgDrawingMethod = drawingMethod;

}

void Level::setDimension(unsigned columns, unsigned rows)
{
    m_columns = columns;
    m_rows = rows;
    m_unit = m_view.getWidth() / m_columns;
}

void Level::setView(irr::core::rect<irr::s32> view)
{
    m_view = view;
}

irr::core::rect<irr::s32> Level::getView() const
{
    return m_view;
}

void Level::update()
{
    if (m_bg)
    {
        irr::core::dimension2du dim = m_bg->getOriginalSize();
        irr::core::rect<irr::s32> srcRect = {{0,0}, dim};
        irr::core::rect<irr::s32> destRect = {0,0,0,0};
        unsigned xRepeats = 1, yRepeats = 1;

        switch (m_bgDrawingMethod)
        {
        case bgDrawingMethod::STRETCH:
            destRect = {0, 0, (irr::s32)(m_columns*m_unit), (irr::s32)(m_rows*m_unit)};
            break;

        case bgDrawingMethod::VERTICAL:
            destRect = {0, 0, m_view.getWidth(), (irr::s32)(dim.Height*(m_view.getWidth()/dim.Width))};
            yRepeats = m_view.getHeight() / destRect.getHeight() + 1;
            break;

        case bgDrawingMethod::HORIZONTAL:
            destRect = {0, 0, (irr::s32)(dim.Width*(m_view.getHeight()/dim.Height)), m_view.getHeight()};
            xRepeats = m_view.getWidth() / destRect.getWidth() + 1;
            break;

        case bgDrawingMethod::TILE:
            destRect = srcRect;
            xRepeats = m_view.getWidth() / dim.Width + 1;
            yRepeats = m_view.getHeight() / dim.Height + 1;
            break;

        default:
            destRect = srcRect;
            break;
        }

        irr::core::position2d<irr::s32> viewOffset = {m_view.UpperLeftCorner.X % destRect.getWidth(), m_view.UpperLeftCorner.Y % destRect.getHeight()};
        destRect -= viewOffset; // moving the background starting position according to the view

        unsigned xr, yr;
        for (xr = 0; xr < xRepeats; ++xr) // column repeats
        {
            for (yr = 0; yr < yRepeats; ++yr) // row repeats
            {
                m_globals->driver->draw2DImage(m_bg, destRect, srcRect);
                destRect += {0, destRect.getHeight()};
            }
            destRect += {destRect.getWidth(),
                (irr::s32)(-yr * destRect.getHeight())}; // restore previous Y position
        }
    }

    for (Element* element : m_elements) element->update();
}
