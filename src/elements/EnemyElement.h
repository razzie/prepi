#ifndef ENEMYELEMENT_H_INCLUDED
#define ENEMYELEMENT_H_INCLUDED

#include "Element.h"

class EnemyElement : public Element
{
public:
    EnemyElement(Level*, std::istream&);
    EnemyElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, unsigned, BehaviorWrapper, MotionWrapper>);
    EnemyElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, float, unsigned damage, Behavior*, Motion*);
    unsigned getDamage() const;
    //void update(uint32_t elapsedMs);
    //void draw();

private:
    ~EnemyElement();

    unsigned m_damage;
};

#endif // ENEMYELEMENT_H_INCLUDED
