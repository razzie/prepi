#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "GroundElement.h"

using namespace irr;

GroundElement::GroundElement(Level* level, std::istream& stream)
 : GroundElement(level, Parser(stream).getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Motion>())
{
}

GroundElement::GroundElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Motion> data)
 : GroundElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

GroundElement::GroundElement(Level* level, unsigned id,
                             irr::core::vector2di position,
                             irr::core::vector2df realCoord,
                             Motion motion)
 : Element(level, Type::GROUND, id, position, realCoord, motion)
{
}

GroundElement::~GroundElement()
{
}
