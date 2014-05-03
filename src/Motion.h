#ifndef MOTION_H_INCLUDED
#define MOTION_H_INCLUDED

#include <iostream>

class Element;

class Motion
{
public:
    enum class Type : unsigned
    {
        NONE = 0,
        STATIC = 3,
        UNSTABLE = 4,
        DYNAMIC = 5,
        AUTO = 6,
        STRAIGHT = 7,
        CIRCULAR = 8
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
