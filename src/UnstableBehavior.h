#ifndef UNSTABLEBEHAVIOR_H_INCLUDED
#define UNSTABLEBEHAVIOR_H_INCLUDED

//#include <tuple>
#include "Behavior.h"

class UnstableBehavior : public Behavior
{
public:
    UnstableBehavior(Element*, std::istream&);
    UnstableBehavior(Element*, bool disappear);
    ~UnstableBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    bool m_triggered;
    bool m_disappear;
};

#endif // UNSTABLEBEHAVIOR_H_INCLUDED
