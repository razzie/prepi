#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED

class Element;

struct Collision
{
    enum Direction
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    Element*  otherElement;
    float     otherElementAngle;
    Direction otherElementDirection;
};

#endif // COLLISION_H_INCLUDED
