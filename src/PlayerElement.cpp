#include "Globals.h"
#include "Level.h"
#include "PlayerElement.h"

using namespace irr;

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

void PlayerElement::draw()
{
    core::recti box = getBoundingBox();
    box -= m_level->getView().UpperLeftCorner;

    m_level->getGlobals()->driver->draw2DRectangle({255,0,0,0}, box);
}
