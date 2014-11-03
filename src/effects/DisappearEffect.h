#ifndef DISAPPEAREFFECT_H_INCLUDED
#define DISAPPEAREFFECT_H_INCLUDED

#include "irrlicht.h"
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
    irr::core::vector2df m_pos;
    uint32_t m_elapsed;
    uint32_t m_duration;
    bool m_animation;
};

#endif // DISAPPEAREFFECT_H_INCLUDED
