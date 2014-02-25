#include "Globals.h"
#include "Level.h"
#include "EnemyElement.h"

using namespace irr;

EnemyElement::EnemyElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion, unsigned> data)
 : EnemyElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data))
{
}

EnemyElement::EnemyElement(Level* level, unsigned id,
                           irr::core::vector2di position, irr::core::vector2df realCoord,
                           Visibility visibility, Motion motion, unsigned damage)
 : Element(level, Type::ENEMY, id, position, realCoord)
 , m_visibility(visibility)
 , m_motion(motion)
 , m_damage(damage)
{
    std::cout << "new EnemyElement" << std::endl;
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
    unsigned unit = m_level->getUnitSize();
    core::recti box = {m_realCoord.X*unit, m_realCoord.Y*unit, m_realCoord.X*unit+unit, m_realCoord.Y*unit+unit};
    box -= m_level->getView().UpperLeftCorner;

    m_level->getGlobals()->driver->draw2DRectangle({255,255,0,0}, box);
}
