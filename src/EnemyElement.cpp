#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "EnemyElement.h"

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
 : Element(level, Type::ENEMY, id, imgPosition, position, behavior, motion)
 , m_damage(damage)
{
    setAnimSpeed(animSpeed);
}

EnemyElement::~EnemyElement()
{
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}
