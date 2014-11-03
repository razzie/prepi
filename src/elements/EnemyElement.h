#ifndef ENEMYELEMENT_H_INCLUDED
#define ENEMYELEMENT_H_INCLUDED

#include "level\TileSet.h"
#include "Element.h"

class EnemyElement : public Element
{
public:
    EnemyElement(Level*, std::istream&);
    EnemyElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper, MotionWrapper, unsigned>);
    EnemyElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, float, float, Behavior*, Motion*, unsigned damage);
    unsigned getDamage() const;
    void update(uint32_t elapsedMs);
    void draw();

private:
    ~EnemyElement();

    unsigned m_damage;
    TileData::Animation::Type m_lastAnimType;
    bool m_lastDirectionLeft;
    bool m_movementAnims;
};

#endif // ENEMYELEMENT_H_INCLUDED
