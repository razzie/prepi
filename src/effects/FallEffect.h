#ifndef FALLEFFECT_H_INCLUDED
#define FALLEFFECT_H_INCLUDED

#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class FallEffect : public ElementEffect
{
public:
    FallEffect(Element*);
    ~FallEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
};

#endif // FALLEFFECT_H_INCLUDED
