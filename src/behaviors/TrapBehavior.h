/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef TRAPBEHAVIOR_H_INCLUDED
#define TRAPBEHAVIOR_H_INCLUDED

#include "behaviors\Behavior.h"

class TrapBehavior : public Behavior
{
public:
    TrapBehavior(Element*);
    ~TrapBehavior();
    void update(uint32_t elapsedMs);
};

#endif // TRAPBEHAVIOR_H_INCLUDED
