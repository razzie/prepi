#ifndef FINISHELEMENT_H_INCLUDED
#define FINISHELEMENT_H_INCLUDED

#include "Element.h"

class FinishElement : public Element
{
public:
    FinishElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df>);
    FinishElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df);
    ~FinishElement();

    //void update();
    void draw();
};

#endif // FINISHELEMENT_H_INCLUDED
