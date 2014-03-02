#include "Globals.h"
#include "Level.h"
#include "RewardElement.h"

using namespace irr;

RewardElement::RewardElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned> data)
 : RewardElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

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
