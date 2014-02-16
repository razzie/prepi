#include "RewardElement.h"

RewardElement::RewardElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord,
                             Motion motion, unsigned value)
 : Element(level, Type::REWARD, id, position, realCoord)
 , m_motion(motion)
 , m_value(value)
{
}

RewardElement::~RewardElement()
{
}

Motion RewardElement::getMotion() const
{
    return m_motion;
}

unsigned RewardElement::getValue() const
{
    return m_value;
}

void RewardElement::update()
{

}
