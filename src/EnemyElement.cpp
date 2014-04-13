#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::istream& stream)
 : EnemyElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df, MotionWrapper, unsigned>())
{
}

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, MotionWrapper, unsigned> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data).motion,
    std::get<4>(data))
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di imgPosition, core::vector2df position,
                           Motion* motion, unsigned damage)
 : Element(level, Type::ENEMY, id, imgPosition, position, new Motion(this, Motion::Type::STATIC))
 , m_damage(damage)
{
    if (motion != nullptr) delete motion; // remporary code!!

    //m_motion->setElement(this); // uncomment this if the line above is removed
}

EnemyElement::~EnemyElement()
{
}

unsigned EnemyElement::getDamage() const
{
    return m_damage;
}
