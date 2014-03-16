#ifndef ENEMYELEMENT_H_INCLUDED
#define ENEMYELEMENT_H_INCLUDED

#include "Element.h"

class EnemyElement : public Element
{
public:
    EnemyElement(Level*, std::istream&);
    EnemyElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion, unsigned>);
    EnemyElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion, unsigned damage);
    Visibility getVisibility() const;
    unsigned getDamage() const;
    //void update();
    //void draw();

private:
    ~EnemyElement();

    Visibility m_visibility;
    unsigned m_damage;
};

#endif // ENEMYELEMENT_H_INCLUDED
