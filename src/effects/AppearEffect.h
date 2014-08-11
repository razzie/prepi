#ifndef APPEAREFFECT_H_INCLUDED
#define APPEAREFFECT_H_INCLUDED

#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class AppearEffect : public ElementEffect
{
public:
    AppearEffect(Element*);
    ~AppearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
};

#endif // APPEAREFFECT_H_INCLUDED
