#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "RewardElement.h"

using namespace irr;

RewardElement::RewardElement(Level* level, std::istream& stream)
 : RewardElement(level, Parser(stream).getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned>())
{
}

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
                             irr::core::vector2di imgPosition, core::vector2df position,
                             Motion motion, unsigned value)
 : Element(level, Type::REWARD, id, imgPosition, position, Motion::STATIC)
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
