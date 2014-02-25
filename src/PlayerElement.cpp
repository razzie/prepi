#include "PlayerElement.h"

PlayerElement::PlayerElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df> data)
 : PlayerElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

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
