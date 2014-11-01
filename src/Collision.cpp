#include "Box2D\Box2D.h"
#include "Collision.h"
#include "elements\Element.h"

using namespace irr;

static float alignedAngle(float angle)
{
    if (angle < 0)
        return (angle + 360);
    else if (angle >= 360)
        return (angle - 360);
    else
        return angle;
}

static float getAngleFromElementContact(Element* element, core::vector2df contactPos)
{
    core::vector2df elemPos = element->getPosition() + element->getBoundingBox().getCenter();
    return alignedAngle( (contactPos - elemPos).getAngleTrig() );
}

static core::vector2df getContactCenterPoint(b2Contact* contact)
{
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    core::vector2df contactPos(0.f, 0.f);
    b2Vec2* points = worldManifold.points;
    unsigned pointCount = contact->GetManifold()->pointCount;

    if (pointCount == 1)
    {
        return {points[0].x + 1.f, points[0].y + 1.f};
    }
    else
    {
        for (unsigned i = 0; i < pointCount; ++i)
        {
            contactPos.X += points[i].x + 1.f;
            contactPos.Y += points[i].y + 1.f;
        }
        contactPos /= pointCount;

        return contactPos;
    }

    //return {worldManifold.points[0].x + 1.f, worldManifold.points[0].y + 1.f};
}

Collision::Collision(Element* otherElement, core::vector2df contactPoint, float otherElementAngle)
 : Collision(otherElement, contactPoint, otherElementAngle,
             getDirectionFromAngle(otherElementAngle))
{
}

Collision::Collision(Element* otherElement, core::vector2df contactPoint, float otherElementAngle, Direction otherElementDirection)
 : m_otherElement(otherElement)
 , m_otherElementAngle(otherElementAngle)
 , m_otherElementDirection(otherElementDirection)
 , m_contactPoint(contactPoint)
{
}

Collision::Collision(const Collision& collision)
 : m_otherElement(collision.m_otherElement)
 , m_otherElementAngle(collision.m_otherElementAngle)
 , m_otherElementDirection(collision.m_otherElementDirection)
 , m_contactPoint(collision.m_contactPoint)
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

core::vector2df Collision::getContactPoint() const
{
    return m_contactPoint;
}

void Collision::updateElementCollisions(Element* element, std::vector<Collision>& collisions, bool clearPrevious)
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

        core::vector2df contactPoint = getContactCenterPoint(edge->contact);
        float contactAngle = getAngleFromElementContact(element, contactPoint);
        collisions.push_back( {contactElem, contactPoint, contactAngle} );
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
