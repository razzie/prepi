#ifndef GROUNDELEMENT_H_INCLUDED
#define GROUNDELEMENT_H_INCLUDED

#include "Element.h"
#include "Motion.h"

class GroundElement : public Element
{
public:
    GroundElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion>);
    GroundElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion);
    ~GroundElement();

    Visibility getVisibility() const;
    Motion getMotion() const;

    //void update();
    //void draw();

private:
    Visibility m_visibility;
    Motion m_motion;
};

#endif // GROUNDELEMENT_H_INCLUDED
