#ifndef APPEAREFFECT_H_INCLUDED
#define APPEAREFFECT_H_INCLUDED

#include "irrlicht.h"
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
    irr::core::vector2df m_pos;
    uint32_t m_elapsed;
    uint32_t m_duration;
    bool m_animation;
};

#endif // APPEAREFFECT_H_INCLUDED
