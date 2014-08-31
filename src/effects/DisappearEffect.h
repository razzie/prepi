#ifndef DISAPPEAREFFECT_H_INCLUDED
#define DISAPPEAREFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class DisappearEffect : public ElementEffect
{
public:
    DisappearEffect(Element*, uint32_t duration = 250);
    ~DisappearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
    uint32_t m_duration;
    irr::core::vector2di m_pos;
};

#endif // DISAPPEAREFFECT_H_INCLUDED
