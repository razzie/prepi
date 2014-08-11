#ifndef DISAPPEAREFFECT_H_INCLUDED
#define DISAPPEAREFFECT_H_INCLUDED

#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class DisappearEffect : public ElementEffect
{
public:
    DisappearEffect(Element*);
    ~DisappearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
};

#endif // DISAPPEAREFFECT_H_INCLUDED
