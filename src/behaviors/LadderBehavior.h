/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef LADDERBEHAVIOR_H_INCLUDED
#define LADDERBEHAVIOR_H_INCLUDED

#include "behaviors\Behavior.h"

class LadderBehavior : public Behavior
{
public:
    LadderBehavior(Element*);
    ~LadderBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);
};

#endif // LADDERBEHAVIOR_H_INCLUDED
