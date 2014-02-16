#include "EnemyElement.h"

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di position, irr::core::vector2df realCoord,
                           Visibility visibility, Motion motion, unsigned damage)
 : Element(level, Type::ENEMY, id, position, realCoord)
 , m_visibility(visibility)
 , m_motion(motion)
 , m_damage(damage)
{
}

EnemyElement::~EnemyElement()
{
}

Element::Visibility EnemyElement::getVisibility() const
{
    return m_visibility;
}

Motion EnemyElement::getMotion() const
{
    return m_motion;
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}

void EnemyElement::update()
{

}
