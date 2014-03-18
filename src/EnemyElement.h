#ifndef ENEMYELEMENT_H_INCLUDED
#define ENEMYELEMENT_H_INCLUDED

#include "Element.h"

class EnemyElement : public Element
{
public:
    EnemyElement(Level*, std::istream&);
    EnemyElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned>);
    EnemyElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned damage);
    unsigned getDamage() const;
    //void update();
    //void draw();

private:
    ~EnemyElement();

    unsigned m_damage;
};

#endif // ENEMYELEMENT_H_INCLUDED
