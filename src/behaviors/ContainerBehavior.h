#ifndef CONTAINERBEHAVIOR_H_INCLUDED
#define CONTAINERBEHAVIOR_H_INCLUDED

//#include <tuple>
#include <string>
#include "behaviors\Behavior.h"

class ContainerBehavior : public Behavior
{
public:
    ContainerBehavior(Element*, std::istream&);
    ContainerBehavior(Element*, std::string elemData);
    ~ContainerBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    Level* m_level;
    std::string m_elemData;
};

#endif // CONTAINERBEHAVIOR_H_INCLUDED
