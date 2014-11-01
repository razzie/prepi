#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <vector>
#include "irrlicht.h"

class Element;

class Collision
{
public:
    enum Direction
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    Collision(Element* otherElement, irr::core::vector2df, float);
    Collision(Element* otherElement, irr::core::vector2df, float, Direction);
    Collision(const Collision&);
    ~Collision();
    Element*  getOtherElement() const;
    float     getOtherElementAngle() const;
    Direction getOtherElementDirection() const;
    irr::core::vector2df getContactPoint() const;

    static void updateElementCollisions(Element*, std::vector<Collision>&, bool clearPrevious = true);
    static Direction getDirectionFromAngle(float angle, float leftRightThresholdAngle = 90.f);

private:
    Element*  m_otherElement;
    float     m_otherElementAngle;
    Direction m_otherElementDirection;
    irr::core::vector2df m_contactPoint;
};

#endif // COLLISION_H_INCLUDED
