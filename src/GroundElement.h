#ifndef GROUNDELEMENT_H_INCLUDED
#define GROUNDELEMENT_H_INCLUDED

#include "Element.h"

class GroundElement : public Element
{
public:
    GroundElement(Level*, std::istream&);
    GroundElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Motion>);
    GroundElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Motion);
    //void update();
    //void draw();

private:
    ~GroundElement();
};

#endif // GROUNDELEMENT_H_INCLUDED
