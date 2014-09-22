#ifndef LEAFEFFECT_H_INCLUDED
#define LEAFEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\EffectManager.h"
#include "effects\ElementEffect.h"

class Element;

class LeafEffect : public ElementEffect
{
public:
    LeafEffect(Element*, irr::video::SColor, irr::core::vector2df);
    ~LeafEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    irr::video::SColor m_color;
    irr::core::vector2df m_direction;
    uint32_t m_elapsed;
};

#endif // LEAFEFFECT_H_INCLUDED
