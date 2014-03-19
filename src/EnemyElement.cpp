#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::istream& stream)
 : EnemyElement(level, Parser(stream).getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned>())
{
}

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di imgPosition, core::vector2df position,
                           Motion motion, unsigned damage)
 : Element(level, Type::ENEMY, id, imgPosition, position, Motion::STATIC)
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
