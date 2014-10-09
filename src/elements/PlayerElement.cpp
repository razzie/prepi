#include <cwchar>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "EventListener.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "elements\PlayerElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"

using namespace irr;

PlayerElement::PlayerElement(Level* level, std::istream& stream)
 : PlayerElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper>())
{
}

PlayerElement::PlayerElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper> data)
 : PlayerElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior)
{
}

PlayerElement::PlayerElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float scale, float animSpeed, Behavior* behavior)
 : Element(level, Type::PLAYER, id, imgPosition, position, scale, animSpeed, behavior, new Motion(this, Motion::Type::DYNAMIC))
 , m_health(100)
 , m_rewards(0)
 , m_speed(2.f)
 , m_lastVelocity(0.f, 0.f)
 , m_injury(0)
 , m_immortalLeft(0)
 , m_lastAnimType(TileData::Animation::Type::RIGHT)
 , m_onLadder(false)
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
    wchar_t text[10];
    swprintf(text, 10, L"+%u", health);

    m_level->getEffectManager()->text(text, this, {255, 0, 255, 0});

    m_health += health;
    if (m_health > getMaxHealth()) m_health = getMaxHealth();
}

void PlayerElement::takeReward(unsigned reward)
{
    wchar_t text[10];
    swprintf(text, 10, L"+%u", reward);

    m_level->getEffectManager()->text(text, this, {255, 255, 255, 0});

    m_rewards += reward;
}

void PlayerElement::takeRewardFrom(RewardElement* reward)
{
    takeReward(reward->getValue());
    reward->remove();
}

void PlayerElement::takeDamage(unsigned dmg)
{
    if (m_immortalLeft > 0) return;

    wchar_t text[10];
    swprintf(text, 10, L"-%u", dmg);

    m_level->getEffectManager()->playerDamage();
    m_level->getEffectManager()->text(text, this, {255, 255, 0, 0});
    m_injury = 255;

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
    else if (it->second.peekElapsed() >= 500)
    {
        takeDamage(enemy->getDamage());
        it->second.reset();
    }
}

void PlayerElement::die()
{
    m_level->getEffectManager()->text("DEAD", this, {255, 255, 0, 0});
    //std::cout << "Player died!" << std::endl;
}

void PlayerElement::setSpeed(f32 speed)
{
    m_speed = speed;
}

void PlayerElement::setImmortal(uint32_t msec)
{
    m_immortalLeft = msec;
}

void PlayerElement::onLadder()
{
    m_onLadder = true;
}

void PlayerElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    // restore gravity after a ladder
    if (m_body->GetGravityScale() == 0.f)
    {
        m_body->SetGravityScale(1.f);
    }

    bool cohesion = false; // if there is cohesion, the player can jump
    bool leftContact = false;
    bool rightContact = false;

    updateCollisions();

    for (auto collision : m_collisions)
    {
        Element* contactElem = collision.getOtherElement();

        switch (contactElem->getType())
        {
            case Element::Type::ENEMY:
                takeDamageFrom(static_cast<EnemyElement*>(contactElem));
                break;

            case Element::Type::REWARD:
                takeRewardFrom(static_cast<RewardElement*>(contactElem));
                m_body->SetLinearVelocity({m_lastVelocity.X, m_lastVelocity.Y});
                break;

            case Element::Type::PARTICLE:
                continue; // skip this element
                break;

            default:
                break;
        }

        switch (collision.getOtherElementDirection())
        //switch ( Collision::getDirectionFromAngle(collision.getOtherElementAngle(), 90.f - m_climbTreshold) )
        {
            case Collision::Direction::BOTTOM:
                cohesion = true;
                break;

            case Collision::Direction::RIGHT:
                rightContact = true;
                break;

            case Collision::Direction::LEFT:
                leftContact = true;
                break;

            default:
                break;
        }

        if ((leftContact || rightContact) &&
            contactElem->getBehaviorType() == Behavior::Type::CLIMBING)
        {
            cohesion = true;
        }
    }

    if (cohesion && m_lastVelocity.Y > 2.0f)
    {
        m_level->getEffectManager()->smoke(m_position + core::vector2df(m_scale / 2, m_scale), 0.25f);
    }

    EventListener* l = m_level->getGlobals()->eventListener;
    b2Vec2 movement = m_body->GetLinearVelocity();

    if (m_onLadder)
    {
        m_onLadder = false;
        m_body->SetGravityScale(0.f); // disable gravity;

        if (l->isUp())
        {
            movement.y = -m_speed;
        }
        else if (l->isDown())
        {
            movement.y = m_speed;
        }
        else
        {
            movement.y = 0.f;
        }
    }
    else
    {
        if (l->isUp() && cohesion)
        {
            movement.y = -m_speed * 2.5f;
        }
        else if (l->isDown())
        {
            movement.y += m_speed / 2.f;
        }
    }

    if (l->isLeft() && (cohesion || !leftContact))
    {
        movement.x = -m_speed;
        m_lastAnimType = TileData::Animation::Type::LEFT;
        m_standbyAnim = false;
    }
    else if (l->isRight() && (cohesion || !rightContact))
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
    m_lastVelocity.set(movement.x, movement.y);

    if (elapsedMs > m_injury) m_injury = 0;
    else m_injury -= elapsedMs;

    if (elapsedMs > m_immortalLeft) m_immortalLeft = 0;
    else m_immortalLeft -= elapsedMs;
}

void PlayerElement::draw()
{
    m_tileData->drawAnimation(m_lastAnimType, (unsigned)(m_animSpeed * 10), m_level, m_imgPosition,
                              m_position, m_scale, m_standbyAnim,
                              video::SColor(255, 255, 255 - m_injury, 255 - m_injury));
}
