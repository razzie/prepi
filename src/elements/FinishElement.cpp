#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\FinishElement.h"

using namespace irr;

FinishElement::FinishElement(Level* level, std::istream& stream)
 : FinishElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, float>())
{
}

FinishElement::FinishElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float> data)
 : FinishElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

FinishElement::FinishElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float animSpeed)
 : Element(level, Type::FINISH, id, imgPosition, position, animSpeed, nullptr, new Motion(this, Motion::Type::STATIC))
{
}

FinishElement::~FinishElement()
{
}

void FinishElement::draw()
{
    m_tileData->drawAnimation(TileData::Animation::Type::RIGHT, (unsigned)(m_animSpeed * 10), m_level, m_imgPosition, m_position);
}
