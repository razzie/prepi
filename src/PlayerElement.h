#ifndef PLAYERELEMENT_H_INCLUDED
#define PLAYERELEMENT_H_INCLUDED

#include "Element.h"

class PlayerElement : public Element
{
public:
    static constexpr irr::f32 NO_CLIMBING = -0.99f;
    static constexpr irr::f32 HALF_CLIMBING = 0.0f;
    static constexpr irr::f32 FULL_CLIMBING = 0.99f;

    PlayerElement(Level*, std::istream&);
    PlayerElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df>);
    PlayerElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df);
    ~PlayerElement();

    void setSpeed(irr::f32);
    void setClimbingMode(irr::f32);

    void update();
    //void draw();

protected:
    irr::f32 m_speed;
    irr::f32 m_climbTreshold;
};

#endif // PLAYERELEMENT_H_INCLUDED
