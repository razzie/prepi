#include "FinishElement.h"

FinishElement::FinishElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df> data)
 : FinishElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

FinishElement::FinishElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord)
 : Element(level, Type::FINISH, id, position, realCoord)
{
}

FinishElement::~FinishElement()
{
}

void FinishElement::update()
{

}
