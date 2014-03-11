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
 , m_speed(1.2f)
 , m_climbTreshold(NO_CLIMBING)
{
    m_level->getPhysics()->DestroyBody(m_body);
    m_body = nullptr;

    // creating new circular body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.X, position.Y);
    bodyDef.fixedRotation = true; // do not rotate!
    bodyDef.userData = this;
    m_body = level->getPhysics()->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_p.Set(-0.5f, -0.5f);
    circleShape.m_radius = 0.5f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.f;
    m_body->CreateFixture(&fixtureDef);
}

PlayerElement::~PlayerElement()
{
}

void PlayerElement::setSpeed(f32 speed)
{
    m_speed = speed;
}

void PlayerElement::setClimbingMode(irr::f32 climbTreshold)
{
    m_climbTreshold = climbTreshold;
}

void PlayerElement::update()
{
    Element::update();

    bool isContactUnder = false;
    bool isContactLeft = false;
    bool isContactRight = false;

    b2ContactEdge* edges = m_body->GetContactList();
    for (b2ContactEdge* edge = edges; edge != NULL; edge = edge->next)
    {
        Element* contactElem = static_cast<Element*>(edge->other->GetUserData());

        //if (contactElem->getType() == Element::Type::GROUND)
        {
            if (contactElem->getPosition().Y  > m_position.Y - m_climbTreshold)
                isContactUnder = true;

            if (contactElem->getPosition().X > m_position.X)
                isContactRight = true;
            else if (contactElem->getPosition().X < m_position.X)
                isContactLeft = true;

        }
    }



    EventListener* l = m_level->getGlobals()->eventListener;
    b2Vec2 movement = m_body->GetLinearVelocity();

    if (l->IsKeyDown(KEY_KEY_W) && isContactUnder)
    {
        movement.y = -m_speed * 2;
    }
    else if (l->IsKeyDown(KEY_KEY_S))
    {
        movement.y = m_speed;
    }

    if (l->IsKeyDown(KEY_KEY_A) && (isContactUnder || !isContactLeft))
    {
        movement.x = -m_speed;
    }
    else if (l->IsKeyDown(KEY_KEY_D) && (isContactUnder || !isContactRight))
    {
        movement.x = m_speed;
    }

    m_body->SetLinearVelocity(movement);
}
