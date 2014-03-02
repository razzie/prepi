#ifndef ENEMYELEMENT_H_INCLUDED
#define ENEMYELEMENT_H_INCLUDED

#include "Element.h"
#include "Motion.h"

class EnemyElement : public Element
{
public:
    EnemyElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion, unsigned>);
    EnemyElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion, unsigned damage);
    ~EnemyElement();

    Visibility getVisibility() const;
    Motion getMotion() const;
    unsigned getDamage() const;

    //void update();
    //void draw();

private:
    Visibility m_visibility;
    Motion m_motion;
    unsigned m_damage;
};

#endif // ENEMYELEMENT_H_INCLUDED
