#ifndef PLAYERELEMENT_H_INCLUDED
#define PLAYERELEMENT_H_INCLUDED

#include "Element.h"

class PlayerElement : public Element
{
public:
    PlayerElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df>);
    PlayerElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df);
    ~PlayerElement();

    //void update();
    void draw();
};

#endif // PLAYERELEMENT_H_INCLUDED
