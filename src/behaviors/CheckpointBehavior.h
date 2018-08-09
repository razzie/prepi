/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef CHECKPOINTBEHAVIOR_H_INCLUDED
#define CHECKPOINTBEHAVIOR_H_INCLUDED

#include "behaviors\Behavior.h"

class CheckpointBehavior : public Behavior
{
public:
    CheckpointBehavior(Element*);
    ~CheckpointBehavior();
    void update(uint32_t elapsedMs);
};

#endif // CHECKPOINTBEHAVIOR_H_INCLUDED
