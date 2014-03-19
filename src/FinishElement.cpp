#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "FinishElement.h"

using namespace irr;

FinishElement::FinishElement(Level* level, std::istream& stream)
 : FinishElement(level, Parser(stream).getArgs<unsigned, irr::core::vector2di, irr::core::vector2df>())
{
}

FinishElement::FinishElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df> data)
 : FinishElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

FinishElement::FinishElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position)
 : Element(level, Type::FINISH, id, imgPosition, position, Motion::STATIC)
{
}

FinishElement::~FinishElement()
{
}
