#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "RewardElement.h"

using namespace irr;

RewardElement::RewardElement(Level* level, std::istream& stream)
 : RewardElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, unsigned, MotionWrapper>())
{
}

RewardElement::RewardElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, unsigned, MotionWrapper> data)
 : RewardElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data).motion)
{
}

RewardElement::RewardElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             unsigned value, Motion* motion)
 : Element(level, Type::REWARD, id, imgPosition, position, motion)
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
