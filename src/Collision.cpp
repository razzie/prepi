#include "Box2D\Box2D.h"
#include "Collision.h"
#include "elements\Element.h"

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

    b2Body* body = element->getBody();
    if (body == nullptr) return;

    for (b2ContactEdge* edge = body->GetContactList(); edge != NULL; edge = edge->next)
    {
        if (!edge->contact->IsTouching())
            continue;

        b2Body* contactBody = edge->other;
        Element* contactElem = static_cast<Element*>(contactBody->GetUserData());

        if (contactElem == 0 || contactElem == element)
            continue;

        core::vector2df posA = element->getPosition() + element->getBoundingBox().getCenter();
        core::vector2df posB = contactElem->getPosition() + contactElem->getBoundingBox().getCenter();
        float contactAngle = alignedAngle( (posB - posA).getAngleTrig() );

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
        return Direction::LEFT;
    }
    // right
    else if (angleBetweenThreshold(angle, leftRightThresholdAngle))
    {
        return Direction::RIGHT;
    }
    // top
    else if (angleBetweenThreshold(angle + 90, leftRightThresholdAngle))
    {
        return Direction::TOP;
    }
    // bottom
    else// if (angleBetweenThreshold(angle + 90, leftRightThresholdAngle))
    {
        return Direction::BOTTOM;
    }
}
