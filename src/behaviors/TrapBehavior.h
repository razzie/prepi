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
