#include "PlayerElement.h"

PlayerElement::PlayerElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord)
 : Element(level, Type::PLAYER, id, position, realCoord)
{
}

PlayerElement::~PlayerElement()
{
}

void PlayerElement::update()
{

}
