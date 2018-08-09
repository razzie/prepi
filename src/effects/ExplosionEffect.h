/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef EXPLOSIONEFFECT_H_INCLUDED
#define EXPLOSIONEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\Effect.h"

class Level;

class ExplosionEffect : public Effect
{
public:
    ExplosionEffect(Level*, irr::core::vector2df pos, unsigned image, float scale,
                    float particleSize = 0.16f, irr::video::SColor color = {255, 255, 0, 0});
    ~ExplosionEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    Level* m_level;
    irr::core::vector2df m_pos;
    unsigned m_image;
    float m_scale;
    float m_pSize;
    irr::video::SColor m_color;
};

#endif // EXPLOSIONEFFECT_H_INCLUDED
