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

static Collision::Direction getDirectionFromAngle(float angle, float leftRightThresholdAngle)
{
    // left
    if (angleBetweenThreshold(angle + 180, leftRightThresholdAngle))
    {
        return Collision::Direction::LEFT;
    }
    // right
    else if (angleBetweenThreshold(angle, leftRightThresholdAngle))
    {
        return Collision::Direction::RIGHT;
    }
    // top
    else if (angleBetweenThreshold(angle + 90, leftRightThresholdAngle))
    {
        return Collision::Direction::TOP;
    }
    // bottom
    else// if (angleBetweenThreshold(angle + 90, leftRightThresholdAngle))
    {
        return Collision::Direction::BOTTOM;
    }
}

static float getAngleFromElementContact(Element* element, core::vector2df contact)
{
    core::vector2df elemPos = element->getPosition() + element->getBoundingBox().getCenter();
    return alignedAngle( (contact - elemPos).getAngleTrig() );
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
        return {points[0].x, points[0].y};
    }
    else
    {
        for (unsigned i = 0; i < pointCount; ++i)
        {
            contactPos.X += points[i].x;
            contactPos.Y += points[i].y;
        }
        contactPos /= pointCount;

        return contactPos;
    }
}


Collision::Collision(Element* A, Element* B, core::vector2df contact)
 : Collision(A, B, contact, getAngleFromElementContact(A, contact))
{
}

Collision::Collision(Element* A, Element* B, core::vector2df contact, float angle)
 : m_elementA(A)
 , m_elementB(B)
 , m_contact(contact)
 , m_angle(angle)
{
}

Collision::Collision(const Collision& collision)
 : m_elementA(collision.m_elementA)
 , m_elementB(collision.m_elementB)
 , m_contact(collision.m_contact)
 , m_angle(collision.m_angle)
{
}

Collision::~Collision()
{
}

Element* Collision::getElementA() const
{
    return m_elementA;
}

Element* Collision::getElementB() const
{
    return m_elementB;
}

core::vector2df Collision::getContactPoint() const
{
    return m_contact;
}

float Collision::getAngle() const
{
    return m_angle;
}

Collision::Direction Collision::getDirection(float leftRightThresholdAngle) const
{
    return getDirectionFromAngle(m_angle, leftRightThresholdAngle);
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

        collisions.push_back( {element, contactElem, getContactCenterPoint(edge->contact)} );
    }
}
