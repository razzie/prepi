#include <iostream>
#include <fstream>
#include <cstdio>
#include "Level.h"
#include "Element.h"
#include "Background.h"
#include "Globals.h"

using namespace irr;

Level::Level(Globals* globals, std::string url)
 : m_globals(globals)
 , m_view({0,0,800,600})
 , m_unit(64)
 , m_bg(new Background(this))
{
    std::fstream file(url);
    char buffer[1024];

    // background
    unsigned bgId, bgDrawM, columns, rows;

    file.getline(buffer, sizeof(buffer)-1);
    std::sscanf(buffer, "%d,%d,%d,%d", &bgId, &bgDrawM, &columns, &rows);

    //setBackground(bgId, static_cast<bgDrawingMethod>(bgDrawM));
    m_bg->setId(bgId);
    m_bg->setDrawingMethod(static_cast<Background::DrawingMethod>(bgDrawM));
    setDimension({columns, rows});

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

    m_bg->update();

    for (Element* element : m_elements) element->update();
}
