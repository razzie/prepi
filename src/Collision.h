/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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

    Collision(Element* A, Element* B, irr::core::vector2df contact);
    Collision(Element* A, Element* B, irr::core::vector2df contact, float angle);
    Collision(const Collision&);
    ~Collision();
    Element* getElementA() const;
    Element* getElementB() const;
    irr::core::vector2df getContactPoint() const;
    float getAngle() const;
    Direction getDirection(float leftRightThresholdAngle = 90.f) const;

    static void updateElementCollisions(Element*, std::vector<Collision>&, bool clearPrevious = true);

private:
    Element* m_elementA;
    Element* m_elementB;
    irr::core::vector2df m_contact;
    float m_angle;
};

#endif // COLLISION_H_INCLUDED
