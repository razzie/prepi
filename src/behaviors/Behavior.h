#ifndef BEHAVIOR_H_INCLUDED
#define BEHAVIOR_H_INCLUDED

#include <cstdint>
#include <iostream>

class Element;

class Behavior
{
public:
    enum Type
    {
        NONE = 0,
        UNSTABLE = 1,
        EXPLOSIVE = 2,
        TELEPORT = 3,
        RESIZER = 4,
        LEAF_EFFECT = 5,
        CONTAINER = 6,
        VENTILATOR = 7,
        LADDER = 8,
        CLIMBING = 9
    };

    Behavior(Element*, Type = Type::NONE);
    virtual ~Behavior();
    Type getType() const;
    virtual void setElement(Element*);
    Element* getElement();
    const Element* getElement() const;
    virtual void update(uint32_t elapsedMs);

protected:
    Type m_type;
    Element* m_element;
};

struct BehaviorWrapper
{
    Element* element = nullptr;
    Behavior* behavior = nullptr;
};

Behavior* CreateBehavior(Element*, std::istream&);

std::istream& operator>> (std::istream&, Behavior::Type&);
std::istream& operator>> (std::istream&, BehaviorWrapper&);

#endif // BEHAVIOR_H_INCLUDED
