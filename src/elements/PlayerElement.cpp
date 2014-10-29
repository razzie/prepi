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
 , m_checkpoint(false)
 , m_checkpointPos(0.f, 0.f)
 , m_checkpointHealth(0)
 , m_lastVelocity(0.f, 0.f)
 , m_injury(0)
 , m_immortalLeft(0)
 , m_lastAnimType(TileData::Animation::Type::RIGHT)
 , m_onLadder(false)
 , m_lastDirectionLeft(false)
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

    m_level->getEffectManager()->playerDamage();
    m_injury = 255;

    if (dmg >= m_health)
    {
        m_health = 0;
        die();
    }
    else
    {
        wchar_t text[10];
        swprintf(text, 10, L"-%u", dmg);
        m_level->getEffectManager()->text(text, this, {255, 255, 0, 0});

        m_health -= dmg;
    }
}

void PlayerElement::takeDamageFrom(EnemyElement* enemy)
{
    for (Damage& dmg : m_damageList)
    {
        // already damaged when touched this enemy first
        if (dmg.m_enemy == enemy)
        {
            dmg.m_collided = true;
            return;
        }
    }

    m_damageList.push_back({enemy, true});
    takeDamage(enemy->getDamage());
}

void PlayerElement::die()
{
    if (m_checkpoint)
    {
        setPosition(m_checkpointPos);
        m_health = m_checkpointHealth;
    }
    else
    {
        m_level->getEffectManager()->text("DEAD", this, {255, 255, 0, 0});
    }
}

void PlayerElement::setSpeed(f32 speed)
{
    m_speed = speed;
}

void PlayerElement::setImmortal(uint32_t msec)
{
    m_immortalLeft = msec;
}

void PlayerElement::checkpoint()
{
    m_checkpoint = true;
    m_checkpointPos = m_position;
    m_checkpointHealth = m_health;
}

void PlayerElement::onLadder()
{
    m_onLadder = true;
}

void PlayerElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    EventListener* l = m_level->getGlobals()->eventListener;
    b2Vec2 movement = m_body->GetLinearVelocity();

    // restore gravity after a ladder
    if (m_body->GetGravityScale() == 0.f)
    {
        m_body->SetGravityScale(1.f);
    }

    // reset enemy collisions so we can remove non-colliding enemies later
    for (Damage& dmg : m_damageList)
    {
        dmg.m_collided = false;
    }

    bool cohesion = false; // if there is cohesion, the player can jump
    bool climbing = false; // for animation only
    bool movingPlatform = false;
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
                //m_body->SetLinearVelocity({m_lastVelocity.X, m_lastVelocity.Y});
                movement.Set(m_lastVelocity.X, m_lastVelocity.Y);
                break;

            case Element::Type::PARTICLE:
                continue; // skip this element
                break;

            default:
                break;
        }

        switch (collision.getOtherElementDirection())
        {
            case Collision::Direction::BOTTOM:
                if (contactElem->getType() != Element::Type::REWARD)
                {
                    cohesion = true;
                    if (contactElem->getType() == Element::Type::GROUND &&
                        contactElem->getMotionType() != Motion::Type::STATIC)
                    {
                        movingPlatform = true;
                        movement.x = contactElem->getBody()->GetLinearVelocity().x * 500; // I don't why 500, but it works
                    }
                }
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

        // we can stick to elements if they have clmbing behavior
        if ((leftContact || rightContact) &&
            contactElem->getBehaviorType() == Behavior::Type::CLIMBING)
        {
            cohesion = true;
            climbing = true;
        }
    }

    // removing those previos enemies that are not touching us anymore
    for (auto it = m_damageList.begin(); it != m_damageList.end(); )
    {
        if (it->m_collided == false)
            it = m_damageList.erase(it);
        else
            ++it;
    }

    // if player is stopped from a fast falling, make some smoke
    if (cohesion && m_lastVelocity.Y > 2.0f)
    {
        m_level->getEffectManager()->smoke(m_position + core::vector2df(m_scale / 2, m_scale), 0.25f);
    }

    if (l->isLeft() && (cohesion || !leftContact))
    {
        movement.x = -m_speed;
        m_lastDirectionLeft = true;
        if (cohesion) m_lastAnimType = TileData::Animation::Type::LEFT;
    }
    else if (l->isRight() && (cohesion || !rightContact))
    {
        movement.x = m_speed;
        m_lastDirectionLeft = false;
        if (cohesion) m_lastAnimType = TileData::Animation::Type::RIGHT;
    }
    else
    {
        if (!movingPlatform) movement.x = 0.f;
        if (cohesion)
        {
            if (m_lastDirectionLeft)
                m_lastAnimType = TileData::Animation::Type::IDLE_LEFT;
            else
                m_lastAnimType = TileData::Animation::Type::IDLE_RIGHT;
        }
    }

    if (m_onLadder)
    {
        m_onLadder = false;
        m_body->SetGravityScale(0.f); // disable gravity;

        if (l->isUp())
        {
            movement.y = -m_speed;
            m_lastAnimType = TileData::Animation::Type::LADDER_UP;
        }
        else if (l->isDown())
        {
            movement.y = m_speed;
            m_lastAnimType = TileData::Animation::Type::LADDER_DOWN;
        }
        else
        {
            movement.y = 0.f;
            m_lastAnimType = TileData::Animation::Type::LADDER_DOWN;
        }
    }
    else
    {
        if (l->isUp() && cohesion)
        {
            movement.y = -m_speed * 2.5f;
            if (climbing)
            {
                if (m_lastDirectionLeft)
                    m_lastAnimType = TileData::Animation::Type::CLIMB_UP_LEFT;
                else
                    m_lastAnimType = TileData::Animation::Type::CLIMB_UP_RIGHT;
            }
            else
            {
                if (m_lastDirectionLeft)
                    m_lastAnimType = TileData::Animation::Type::JUMP_LEFT;
                else
                    m_lastAnimType = TileData::Animation::Type::JUMP_RIGHT;
            }
        }
        else if (l->isDown())
        {
            movement.y += m_speed / 2.f;
            if (climbing)
            {
                if (m_lastDirectionLeft)
                    m_lastAnimType = TileData::Animation::Type::CLIMB_DOWN_LEFT;
                else
                    m_lastAnimType = TileData::Animation::Type::CLIMB_DOWN_RIGHT;
            }
            else
            {
                if (m_lastDirectionLeft)
                    m_lastAnimType = TileData::Animation::Type::JUMP_LEFT;
                else
                    m_lastAnimType = TileData::Animation::Type::JUMP_RIGHT;
            }
        }
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
    m_tileData->drawAnimation(m_level, m_imgPosition, m_lastAnimType, m_animSpeed, m_position, m_scale, 0.f,
                              video::SColor(255, 255, 255 - m_injury, 255 - m_injury));
}
