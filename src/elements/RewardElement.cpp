#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "elements\RewardElement.h"

using namespace irr;

RewardElement::RewardElement(Level* level, std::istream& stream)
 : RewardElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                    float, unsigned, BehaviorWrapper, MotionWrapper>())
{
}

RewardElement::RewardElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                      float, unsigned, BehaviorWrapper, MotionWrapper> data)
 : RewardElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior,
    std::get<6>(data).motion)
{
}

RewardElement::RewardElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float animSpeed, unsigned value, Behavior* behavior, Motion* motion)
 : Element(level, Type::REWARD, id, imgPosition, position, animSpeed, behavior, motion)
 , m_value(value)
{
}

RewardElement::~RewardElement()
{
}

unsigned RewardElement::getValue() const
{
    return m_value;
}
