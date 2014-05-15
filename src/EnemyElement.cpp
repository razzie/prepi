#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::istream& stream)
 : EnemyElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, unsigned, MotionWrapper>())
{
}

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, unsigned, MotionWrapper> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data).motion)
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di imgPosition, core::vector2df position,
                           unsigned damage, Motion* motion)
 : Element(level, Type::ENEMY, id, imgPosition, position, motion)
 , m_damage(damage)
{
}

EnemyElement::~EnemyElement()
{
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}
