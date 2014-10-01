#ifndef SMOKEEFFECT_H_INCLUDED
#define SMOKEEFFECT_H_INCLUDED

#include <vector>
#include "effects\EffectManager.h"

class Level;

class SmokeEffect : public Effect
{
public:
    SmokeEffect(Level*, irr::core::vector2df pos, float scale);
    ~SmokeEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    struct Smoke
    {
        irr::core::vector2df m_position;
        irr::core::vector2df m_velocity;
        irr::video::SColor m_color;
        uint32_t m_begin;
        uint32_t m_duration;
    };

    static irr::video::ITexture* m_texture;
    Level* m_level;
    irr::core::vector2df m_pos;
    float m_scale;
    uint32_t m_elapsed;
    std::vector<Smoke> m_smokes;
};

#endif // SMOKEEFFECT_H_INCLUDED
