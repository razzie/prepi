#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "EventListener.h"
#include "Level.h"
#include "PlayerElement.h"
#include "EnemyElement.h"
#include "RewardElement.h"

using namespace irr;

PlayerElement::PlayerElement(Level* level, std::istream& stream)
 : PlayerElement(level, Parser(stream).getArgs<unsigned, irr::core::vector2di, irr::core::vector2df>())
{
}

PlayerElement::PlayerElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df> data)
 : PlayerElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

PlayerElement::PlayerElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord)
 : Element(level, Type::PLAYER, id, position, realCoord, Motion::DYNAMIC)
 , m_health(100)
 , m_rewards(0)
 , m_speed(2.f)
 , m_climbTreshold(FULL_CLIMBING)
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
    circleShape.m_radius = 0.48f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 1.f;
    m_body->CreateFixture(&fixtureDef);
}

PlayerElement::~PlayerElement()
{
}

unsigned PlayerElement::getHealth() const
{
    return m_health;
}

unsigned PlayerElement::getMaxHealth() const
{
    return 100;
}

unsigned PlayerElement::getRewards() const
{
    return m_rewards;
}

void PlayerElement::takeHealth(unsigned health)
{
    m_health += health;
    if (m_health > getMaxHealth()) m_health = getMaxHealth();
}

void PlayerElement::takeReward(unsigned reward)
{
    m_rewards += reward;
}

void PlayerElement::takeRewardFrom(RewardElement* reward)
{
    takeReward(reward->getValue());
    reward->remove();
}

void PlayerElement::takeDamage(unsigned dmg)
{
    if (dmg >= m_health)
    {
        m_health = 0;
        die();
    }
    else
    {
        m_health -= dmg;
    }
}

void PlayerElement::takeDamageFrom(EnemyElement* enemy)
{
    auto it = m_damageList.find(enemy);
    if (it == m_damageList.end())
    {
        m_damageList.insert( std::make_pair(enemy, Timer()) );
        takeDamage(enemy->getDamage());
    }
    else if (it->second.peekElapsed() >= 1000)
    {
        takeDamage(enemy->getDamage());
        it->second.reset();
    }
}

void PlayerElement::die()
{
    std::cout << "Player died!" << std::endl;
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

        if (contactElem->getType() == Element::Type::ENEMY)
        {
            takeDamageFrom(static_cast<EnemyElement*>(contactElem));
        }
        else if (contactElem->getType() == Element::Type::REWARD)
        {
            takeRewardFrom(static_cast<RewardElement*>(contactElem));
        }
    }

    EventListener* l = m_level->getGlobals()->eventListener;
    b2Vec2 movement = m_body->GetLinearVelocity();

    if (l->IsUp() && isContactUnder)
    {
        movement.y = -m_speed * 2.2;
    }
    else if (l->IsDown())
    {
        movement.y = m_speed;
    }

    if (l->IsLeft() && (isContactUnder || !isContactLeft))
    {
        movement.x = -m_speed;
    }
    else if (l->IsRight() && (isContactUnder || !isContactRight))
    {
        movement.x = m_speed;
    }
    else
    {
        movement.x = 0.f;
    }

    m_body->SetLinearVelocity(movement);
}
