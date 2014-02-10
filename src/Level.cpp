#include "Level.h"
#include "Element.h"

Level::Level(Globals* globals, std::string url)
{

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
    m_bgId = id;
    m_bgDrawingMethod = drawingMethod;
}

void Level::setDimension(unsigned columns, unsigned rows)
{
    m_columns = columns;
    m_rows = rows;
}

void Level::update()
{
    for (Element* element : m_elements) element->update();
}
