/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

#include <cstdint>

class Effect
{
public:
    virtual ~Effect() {}
    virtual void update(uint32_t elapsedMs) = 0;
    virtual bool isFinished() const = 0;
};

#endif // EFFECT_H_INCLUDED
