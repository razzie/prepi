/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef UNSTABLEBEHAVIOR_H_INCLUDED
#define UNSTABLEBEHAVIOR_H_INCLUDED

#include "behaviors\Behavior.h"

class UnstableBehavior : public Behavior
{
public:
    UnstableBehavior(Element*, std::istream&);
    UnstableBehavior(Element*, bool disappear);
    ~UnstableBehavior();
    void update(uint32_t elapsedMs);

protected:
    bool m_disappear;
};

#endif // UNSTABLEBEHAVIOR_H_INCLUDED
