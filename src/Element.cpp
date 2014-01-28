#include "Element.h"

Element::Element(gameGlobals* globals)
{
    m_globals = globals;
    m_globals->elements.insert(this);
}

Element::~Element()
{
    m_globals->elements.erase(this);
}

irr::core::vector2di Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(irr::core::vector2di position)
{
    m_position = position;
}
