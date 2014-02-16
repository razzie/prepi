#include "GroundElement.h"

GroundElement::GroundElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord,
                             Visibility visibility, Motion motion)
 : Element(level, Type::GROUND, id, position, realCoord)
 , m_visibility(visibility)
 , m_motion(motion)
{
}

GroundElement::~GroundElement()
{
}

Element::Visibility GroundElement::getVisibility() const
{
    return m_visibility;
}

Motion GroundElement::getMotion() const
{
    return m_motion;
}

void GroundElement::update()
{

}
