/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef DELAYEFFECT_H_INCLUDED
#define DELAYEFFECT_H_INCLUDED

#include "effects\Effect.h"

class Level;

class DelayEffect : public Effect
{
public:
    DelayEffect(uint32_t delay, Effect* delayedEffect);
    ~DelayEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_delay;
    uint32_t m_elapsed;
    Effect* m_delayedEffect;
};

#endif // DELAYEFFECT_H_INCLUDED
