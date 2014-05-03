#ifndef FINISHELEMENT_H_INCLUDED
#define FINISHELEMENT_H_INCLUDED

#include "Element.h"

class FinishElement : public Element
{
public:
    FinishElement(Level*, std::istream&);
    FinishElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df>);
    FinishElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df);
    //void update(uint32_t elapsedMs);
    //void draw();

protected:
    ~FinishElement();
};

#endif // FINISHELEMENT_H_INCLUDED
