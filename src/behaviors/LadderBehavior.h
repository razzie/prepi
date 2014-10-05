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
