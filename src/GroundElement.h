#ifndef GROUNDELEMENT_H_INCLUDED
#define GROUNDELEMENT_H_INCLUDED

#include "Element.h"

class GroundElement : public Element
{
public:
    GroundElement(Level*, std::istream&);
    GroundElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion>);
    GroundElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion);
    Visibility getVisibility() const;
    //void update();
    //void draw();

private:
    ~GroundElement();

    Visibility m_visibility;
};

#endif // GROUNDELEMENT_H_INCLUDED
