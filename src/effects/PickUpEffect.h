#ifndef PICKUPEFFECT_H_INCLUDED
#define PICKUPEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class PickUpEffect : public ElementEffect
{
public:
    PickUpEffect(Element*);
    ~PickUpEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
    irr::core::vector2di m_beginPos;
    irr::core::vector2di m_endPos;
};

#endif // PICKUPEFFECT_H_INCLUDED
