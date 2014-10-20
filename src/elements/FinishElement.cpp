#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\FinishElement.h"

using namespace irr;

FinishElement::FinishElement(Level* level, std::istream& stream)
 : FinishElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper>())
{
}

FinishElement::FinishElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper> data)
 : FinishElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior)
{
}

FinishElement::FinishElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float scale, float animSpeed, Behavior* behavior)
 : Element(level, Type::FINISH, id, imgPosition, position, scale, animSpeed, behavior, new Motion(this, Motion::Type::STATIC))
{
}

FinishElement::~FinishElement()
{
}
