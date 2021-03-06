/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef RESIZERBEHAVIOR_H_INCLUDED
#define RESIZERBEHAVIOR_H_INCLUDED

#include "behaviors\Behavior.h"

class ResizerBehavior : public Behavior
{
public:
    ResizerBehavior(Element*, std::istream&);
    ResizerBehavior(Element*, float scale);
    ~ResizerBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    float m_scale;
};

#endif // RESIZERBEHAVIOR_H_INCLUDED
