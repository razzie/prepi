#include "Element.h"
#include "Level.h"

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di position, irr::core::vector2df realCoord)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_position(position)
 , m_realCoord(realCoord)
{
    m_level->addElement(this);
}

Element::~Element()
{
    m_level->removeElement(this);
}

Element::Type Element::getType() const
{
    return m_type;
}

unsigned Element::getId() const
{
    return m_id;
}

irr::core::vector2di Element::getPosition() const
{
    return m_position;
}

irr::core::vector2df Element::getRealCoord() const
{
    return m_realCoord;
}

void Element::setRealCoord(irr::core::vector2df realCoord)
{
    m_realCoord = realCoord;
}
