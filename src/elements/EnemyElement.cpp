#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "elements\EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::istream& stream)
 : EnemyElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                   float, float, BehaviorWrapper, MotionWrapper, unsigned>())
{
}

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                    float, float, BehaviorWrapper, MotionWrapper, unsigned> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior,
    std::get<6>(data).motion,
    std::get<7>(data))
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di imgPosition, core::vector2df position,
                           float scale, float animSpeed, Behavior* behavior, Motion* motion, unsigned damage)
 : Element(level, Type::ENEMY, id, imgPosition, position, scale, animSpeed, behavior, motion)
 , m_damage(damage)
 , m_lastAnimType(TileData::Animation::Type::IDLE_RIGHT)
 , m_lastDirectionLeft(false)
 , m_movementAnims(false)
{
    if (m_tileData != nullptr &&
        m_tileData->getAnimation(m_imgPosition, TileData::Animation::Type::IDLE_RIGHT) != nullptr &&
        m_tileData->getAnimation(m_imgPosition, TileData::Animation::Type::IDLE_LEFT) != nullptr &&
        m_tileData->getAnimation(m_imgPosition, TileData::Animation::Type::RIGHT) != nullptr &&
        m_tileData->getAnimation(m_imgPosition, TileData::Animation::Type::LEFT) != nullptr)
    {
        m_movementAnims = true;
    }
}

EnemyElement::~EnemyElement()
{
    m_level->getEffectManager()->disappear(this);
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}

void EnemyElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    if (m_tileData == nullptr || m_body == nullptr)
        return;

    if (m_movementAnims)
    {
        if (m_lastDirectionLeft)
            m_lastAnimType = TileData::Animation::Type::IDLE_LEFT;
        else
            m_lastAnimType = TileData::Animation::Type::IDLE_RIGHT;

        b2Vec2 movement = m_body->GetLinearVelocity();
        if (movement.x > 0.f) // right
        {
            m_lastDirectionLeft = false;
            m_lastAnimType = TileData::Animation::Type::RIGHT;
        }
        else if (movement.x < 0.f) // left
        {
            m_lastDirectionLeft = true;
            m_lastAnimType = TileData::Animation::Type::LEFT;
        }
    }
}

void EnemyElement::draw()
{
    m_tileData->drawAnimation(m_level, m_imgPosition, (m_movementAnims ? m_lastAnimType : 0), m_animSpeed * m_level->getSpeed(), m_position, m_scale);
}
