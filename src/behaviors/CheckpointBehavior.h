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
