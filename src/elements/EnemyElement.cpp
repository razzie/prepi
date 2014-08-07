#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "elements\EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::istream& stream)
 : EnemyElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                   float, unsigned, BehaviorWrapper, MotionWrapper>())
{
}

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                    float, unsigned, BehaviorWrapper, MotionWrapper> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior,
    std::get<6>(data).motion)
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di imgPosition, core::vector2df position,
                           float animSpeed, unsigned damage, Behavior* behavior, Motion* motion)
 : Element(level, Type::ENEMY, id, imgPosition, position, animSpeed, behavior, motion)
 , m_damage(damage)
 , m_lastAnimType(TileData::Animation::Type::RIGHT)
{
}

EnemyElement::~EnemyElement()
{
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}

void EnemyElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    if (!m_tileData || !m_body) return;

    const TileData::Animation* anim = m_tileData->getAnimation(m_imgPosition);
    if (anim && anim->animCount > 1)
    {
        b2Vec2 velocity = m_body->GetLinearVelocity();
        m_lastAnimType = (velocity.x > 0) ? TileData::Animation::Type::RIGHT : TileData::Animation::Type::LEFT;
    }
}

void EnemyElement::draw()
{
    m_tileData->drawAnimation(m_lastAnimType, (unsigned)(m_animSpeed * 10), m_level, m_imgPosition, m_position);
}