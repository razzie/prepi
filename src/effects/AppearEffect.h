#ifndef APPEAREFFECT_H_INCLUDED
#define APPEAREFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\ElementEffect.h"

class Element;

class AppearEffect : public ElementEffect
{
public:
    AppearEffect(Element*, uint32_t duration = 250);
    ~AppearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
    uint32_t m_duration;
    irr::core::vector2df m_pos;
};

#endif // APPEAREFFECT_H_INCLUDED
