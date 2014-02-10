#include "Element.h"
#include "Level.h"

Element::Element(Level* level)
{
    m_level = level;
    m_level->addElement(this);
}

Element::~Element()
{
    m_level->removeElement(this);
}

irr::core::vector2di Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(irr::core::vector2di position)
{
    m_position = position;
}
