#ifndef MOTION_H_INCLUDED
#define MOTION_H_INCLUDED

#include <cstdint>
#include <iostream>

class Element;

class Motion
{
public:
    enum class Type : unsigned
    {
        NONE = 0,
        STATIC = 3,
        DYNAMIC = 4,
        AUTO = 5,
        STRAIGHT = 6,
        CIRCULAR = 7
    };

    Motion(Element*, Type = Type::NONE);
    virtual ~Motion();
    Type getType() const;
    virtual void setElement(Element*);
    Element* getElement();
    const Element* getElement() const;
    virtual void update(uint32_t elapsedMs);

protected:
    Type m_type;
    Element* m_element;
};

struct MotionWrapper
{
    Element* element = nullptr;
    Motion* motion = nullptr;
};

Motion* CreateMotion(Element*, std::istream&);

std::istream& operator>> (std::istream&, Motion::Type&);
std::istream& operator>> (std::istream&, MotionWrapper&);

#endif // MOTION_H_INCLUDED
