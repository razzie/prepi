/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef DAMAGEEFFECT_H_INCLUDED
#define DAMAGEEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\Effect.h"

class Level;

class DamageEffect : public Effect
{
public:
    DamageEffect(Level*);
    ~DamageEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    static irr::video::ITexture* m_texture;
    Level* m_level;
    uint32_t m_elapsed;
};

#endif // DAMAGEEFFECT_H_INCLUDED
