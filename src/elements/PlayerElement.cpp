#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "EventListener.h"
#include "level\Level.h"
#include "elements\PlayerElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"

using namespace irr;

PlayerElement::PlayerElement(Level* level, std::istream& stream)
 : PlayerElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, float>())
{
}

PlayerElement::PlayerElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float> data)
 : PlayerElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

PlayerElement::PlayerElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float animSpeed)
 : Element(level, Type::PLAYER, id, imgPosition, position, 1.0f, nullptr, new Motion(this, Motion::Type::DYNAMIC))
 , m_health(100)
 , m_rewards(0)
 , m_speed(2.f)
 , m_climbTreshold(FULL_CLIMBING)
 , m_lastAnimType(TileData::Animation::Type::RIGHT)
{
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

void PlayerElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    bool isContactUnder = false;
    bool isContactLeft = false;
    bool isContactRight = false;

    updateCollisions();

    for (auto collision : m_collisions)
    {
        Element* contactElem = collision.getOtherElement();

        contactElem->drawDebugBox();

        switch (contactElem->getType())
        {
            case Element::Type::ENEMY:
                takeDamageFrom(static_cast<EnemyElement*>(contactElem));
                break;

            case Element::Type::REWARD:
                takeRewardFrom(static_cast<RewardElement*>(contactElem));
                break;

            case Element::Type::PARTICLE:
                continue; // skip this element
                break;

            default:
                break;
        }

        //switch (collision.getOtherElementDirection())
        switch ( Collision::getDirectionFromAngle(collision.getOtherElementAngle(), 90.f - m_climbTreshold) )
        {
            case Collision::Direction::BOTTOM:
                isContactUnder = true;
                break;

            case Collision::Direction::RIGHT:
                isContactRight = true;
                break;

            case Collision::Direction::LEFT:
                isContactLeft = true;
                break;

            default:
                break;
        }
    }

    EventListener* l = m_level->getGlobals()->eventListener;
    b2Vec2 movement = m_body->GetLinearVelocity();

    if (l->isUp() && isContactUnder)
    {
        movement.y = -m_speed * 2.2;
    }
    else if (l->isDown())
    {
        movement.y = m_speed;
    }

    if (l->isLeft() && (isContactUnder || !isContactLeft))
    {
        movement.x = -m_speed;
        m_lastAnimType = TileData::Animation::Type::LEFT;
        m_standbyAnim = false;
    }
    else if (l->isRight() && (isContactUnder || !isContactRight))
    {
        movement.x = m_speed;
        m_lastAnimType = TileData::Animation::Type::RIGHT;
        m_standbyAnim = false;
    }
    else
    {
        movement.x = 0.f;
        m_standbyAnim = true;
    }

    m_body->SetLinearVelocity(movement);
}

void PlayerElement::draw()
{
    m_tileData->drawAnimation(m_lastAnimType, (unsigned)(m_animSpeed * 10), m_level, m_imgPosition, m_position, m_standbyAnim);
}
