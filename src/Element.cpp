#include "Element.h"
#include "Level.h"

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, irr::core::vector2df position)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_imgPosition(imgPosition)
 , m_position(position)
{
    irr::s32 unit = (irr::s32)m_level->getUnitSize();
    m_boudingBox = {m_position.X * unit, m_position.Y * unit, m_position.X * unit + unit, m_position.Y * unit + unit};

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

irr::core::vector2di Element::getImagePosition() const
{
    return m_imgPosition;
}

irr::core::vector2df Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(irr::core::vector2df position)
{
    m_position = position;
}

irr::core::recti Element::getBoundingBox() const
{
    return m_boudingBox;
}

void Element::update()
{
}


std::istream& operator>> (std::istream& i, Element::Type& t)
{
    unsigned type;
    i >> type;
    t = static_cast<Element::Type>(type);
    return i;
}

std::istream& operator>> (std::istream& i, Element::Visibility& v)
{
    unsigned visibility;
    i >> visibility;
    v = static_cast<Element::Visibility>(visibility);
    return i;
}

std::istream& operator>> (std::istream& i, irr::core::vector2di& v)
{
    int x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}

std::istream& operator>> (std::istream& i, irr::core::vector2df& v)
{
    float x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}
