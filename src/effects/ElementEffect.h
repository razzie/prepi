#ifndef ELEMENTEFFECT_H_INCLUDED
#define ELEMENTEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\EffectManager.h"

class Element;

// abstract parent class from element related effects
class ElementEffect : public Effect
{
public:
    ElementEffect(Element*);

protected:
    Element* m_element;

    void drawTile(irr::core::vector2di screenPos, float angle = 0.f, uint8_t alpha = 255) const;
    irr::core::vector2di getScreenPosition() const;
};

#endif // ELEMENTEFFECT_H_INCLUDED
