#include "Box2D\Box2D.h"
#include "Globals.h"
#include "EventListener.h"
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

void PlayerElement::update()
{
    Element::update();

    const f32 speed = 3.f;
    EventListener* l = m_level->getGlobals()->eventListener;

    if (l->IsKeyDown(KEY_KEY_W) && m_body->GetLinearVelocity().y == 0)
    {
        setMovementY(-speed * 2);
    }
    else if (l->IsKeyDown(KEY_KEY_S))
    {
        setMovementY(speed);
    }

    if (l->IsKeyDown(KEY_KEY_A))
    {
        setMovementX(-speed);
    }
    else if (l->IsKeyDown(KEY_KEY_D))
    {
        setMovementX(speed);
    }
}
