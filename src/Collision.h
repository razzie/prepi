#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

#include <vector>

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

    Collision(Element*, float);
    Collision(Element*, float, Direction);
    Collision(const Collision&);
    ~Collision();
    Element*  getOtherElement() const;
    float     getOtherElementAngle() const;
    Direction getOtherElementDirection() const;

    static void getElementCollisions(Element*, std::vector<Collision>&, bool clearPrevious = true);
    static Direction getDirectionFromAngle(float angle, float leftRightThresholdAngle = 90.f);

private:
    Element*  m_otherElement;
    float     m_otherElementAngle;
    Direction m_otherElementDirection;
};

#endif // COLLISION_H_INCLUDED
