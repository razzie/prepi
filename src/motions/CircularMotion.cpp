#include <algorithm>
#include <cmath>
#include "Box2D\Box2D.h"
#include "Parser.h"
#include "motions\CircularMotion.h"
#include "elements\Element.h"

#define PI 3.14159265358979323846

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

CircularMotion::CircularMotion(Element* element, std::istream& stream)
 : CircularMotion(element, Parser(stream, ',').getArgs<float, uint32_t, bool, int, core::vector2df>())
{
}

CircularMotion::CircularMotion(Element* element, std::tuple<float, uint32_t, bool, int, core::vector2df> data)
 : CircularMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

CircularMotion::CircularMotion(Element* element, float speed, uint32_t delay, bool ai, int angle, core::vector2df center)
 : ActiveMotion(element, Type::STRAIGHT, speed, delay, ai)
 , m_angle(angle)
 , m_radius(0.f)
 , m_angleSpeedRad( ((float)m_speed / 180.f) * PI )
 , m_startAngleRad(0.f)
 , m_center(center)
{
    if (m_angle < 0)
    {
        m_angleSpeedRad *= -1;
        m_angle *= -1;
    }
}

CircularMotion::~CircularMotion()
{
}

core::vector2df CircularMotion::getCenter() const
{
    return m_center;
}

int CircularMotion::getAngle() const
{
    return m_angle;
}

void CircularMotion::setElement(Element* element)
{
    Motion::setElement(element);

    if (element != nullptr)
    {
        float scale = element->getScale();
        core::vector2df pos = element->getPosition();
        pos += core::vector2df(scale / 2.f, scale / 2.f); // let's move the element's (top-left) origo to its center

        double startAngle = -((pos - m_center).getAngleTrig()) + 90.f;

        m_radius = pos.getDistanceFrom(m_center);
        m_startAngleRad = startAngle / 180.f * PI;
    }
}

void CircularMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    if (m_elapsed <= m_delay || m_element == nullptr) return;

    uint32_t travelInterval = 1000 * m_angle / m_speed;
    uint32_t alignedElapsedMs = (m_elapsed - m_delay) % (travelInterval * 2);

    float elapsedSec;

    if (m_angle == 360 || alignedElapsedMs <= travelInterval)
    {
        elapsedSec = (float)alignedElapsedMs / 1000.f;
    }
    else
    {
        elapsedSec = (float)(travelInterval - (alignedElapsedMs - travelInterval)) / 1000.f;
    }

    // calculating new position
    float scale = m_element->getScale();
    core::vector2df pos = m_center;
    pos.X += m_radius * std::sin(m_startAngleRad + m_angleSpeedRad * elapsedSec);
    pos.Y += m_radius * std::cos(m_startAngleRad + m_angleSpeedRad * elapsedSec);
    pos -= core::vector2df(scale / 2.f, scale / 2.f); // pos points to the center of element, moving it to the top-left corner

    // calculating velocity vector
    core::vector2df velocity = m_element->getPosition(); // it will be rotated by 90 or -90 degrees first
    std::swap(velocity.X, velocity.Y);
    if (m_speed < 0) velocity.X *= -1;
    else velocity.Y *= -1;
    velocity.normalize();
    velocity *= m_angleSpeedRad; // radius is missing, but it's better this way

    m_element->setPosition(pos);
    m_element->getBody()->SetLinearVelocity({velocity.X, velocity.Y});
}
