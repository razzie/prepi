#include "FinishElement.h"

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
