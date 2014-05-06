#include "Box2D\Box2D.h"
#include "Collision.h"
#include "Element.h"

using namespace irr;

Collision::Collision(Element* otherElement, float otherElementAngle)
 : m_otherElement(otherElement)
 , m_otherElementAngle(otherElementAngle)
 , m_otherElementDirection(getDirectionFromAngle(otherElementAngle))
{
}

Collision::Collision(Element* otherElement, float otherElementAngle, Direction otherElementDirection)
 : m_otherElement(otherElement)
 , m_otherElementAngle(otherElementAngle)
 , m_otherElementDirection(otherElementDirection)
{
}

Collision::Collision(const Collision& collision)
 : m_otherElement(collision.m_otherElement)
 , m_otherElementAngle(collision.m_otherElementAngle)
 , m_otherElementDirection(collision.m_otherElementDirection)
{
}

Collision::~Collision()
{
}

Element* Collision::getOtherElement() const
{
    return m_otherElement;
}

float Collision::getOtherElementAngle() const
{
    return m_otherElementAngle;
}

Collision::Direction Collision::getOtherElementDirection() const
{
    return m_otherElementDirection;
}

static float alignedAngle(float angle)
{
    if (angle < 0)
        return (angle + 360);
    else if (angle >= 360)
        return (angle - 360);
    else
        return angle;
}

void Collision::getElementCollisions(Element* element, std::vector<Collision>& collisions, bool clearPrevious)
{
    if (clearPrevious) collisions.clear();

    b2ContactEdge* edges = element->getBody()->GetContactList();

    for (b2ContactEdge* edge = edges; edge != NULL; edge = edge->next)
    {
        b2Body* contactBody = edge->other;
        Element* contactElem = static_cast<Element*>(contactBody->GetUserData());

        if (contactElem == 0 || contactElem == element)
            continue;

        b2Vec2 posA = element->getBody()->GetPosition();
        b2Vec2 posB = contactBody->GetPosition();
        float contactAngle = alignedAngle( (core::vector2df(posB.x - posA.x, posB.y - posA.y).getAngleTrig()) /*+ 90.f*/ );

        collisions.push_back({contactElem, contactAngle});
    }
}

static bool angleBetween(float angle, float minAngle, float maxAngle)
{
    angle = alignedAngle(angle);
    minAngle = alignedAngle(minAngle);
    maxAngle = alignedAngle(maxAngle);

    if (maxAngle >= minAngle)
    {
        return (angle >= minAngle && angle <= maxAngle);
    }
    else
    {
        return ((angle >= minAngle && angle < 360) ||
                (angle >= 0 && angle <= maxAngle));
    }
}

static bool angleBetweenThreshold(float angle, float threshold)
{
    return angleBetween(angle, -threshold/2, threshold/2);
}

Collision::Direction Collision::getDirectionFromAngle(float angle, float leftRightThresholdAngle)
{
    // left
    if (angleBetweenThreshold(angle + 180, leftRightThresholdAngle))
    {
        //std::cout << "angle: " << angle << " left" << std::endl;
        return Direction::LEFT;
    }
    // right
    else if (angleBetweenThreshold(angle, leftRightThresholdAngle))
    {
        //std::cout << "angle: " << angle << " right" << std::endl;
        return Direction::RIGHT;
    }
    // top
    else if (angleBetweenThreshold(angle + 270, leftRightThresholdAngle))
    {
        //std::cout << "angle: " << angle << " top" << std::endl;
        return Direction::TOP;
    }
    // bottom
    else// if (angleBetweenThreshold(angle + 90, leftRightThresholdAngle))
    {
        //std::cout << "angle: " << angle << " bottom" << std::endl;
        return Direction::BOTTOM;
    }
}
