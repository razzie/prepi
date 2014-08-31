#ifndef EXPLOSIONEFFECT_H_INCLUDED
#define EXPLOSIONEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\EffectManager.h"

class Level;

class ExplosionEffect : public Effect
{
public:
    ExplosionEffect(Level*, irr::core::vector2df pos, float range,
                    irr::video::SColor color = irr::video::SColor(255, 255, 0, 0));
    ~ExplosionEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    Level* m_level;
    irr::core::vector2df m_pos;
    float m_range;
    irr::video::SColor m_color;
};

#endif // EXPLOSIONEFFECT_H_INCLUDED
