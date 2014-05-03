#include <cmath>
#include "Parser.h"
#include "CircularMotion.h"
#include "Element.h"

#define PI 3.14159265358979323846

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

CircularMotion::CircularMotion(Element* element, std::istream& stream)
 : CircularMotion(element, Parser(stream, ',').getArgs<uint32_t, uint32_t, bool, int, core::vector2df>())
{
}

CircularMotion::CircularMotion(Element* element, std::tuple<uint32_t, uint32_t, bool, int, core::vector2df> data)
 : CircularMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

CircularMotion::CircularMotion(Element* element, uint32_t speed, uint32_t delay, bool ai, int angle, core::vector2df center)
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

    core::vector2df pos = element->getPosition();
    double startAngle = -((pos - m_center).getAngleTrig()) + 90.f;

    m_radius = pos.getDistanceFrom(m_center);
    m_startAngleRad = startAngle / 180.f * PI;
}

void CircularMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    int travelInterval = 1000 * m_angle / m_speed;
    int alignedElapsedMs = m_elapsed % (travelInterval * 2);

    float elapsedSec;

    if (m_angle == 360 || alignedElapsedMs <= travelInterval)
    {
        elapsedSec = (float)alignedElapsedMs / 1000.f;
    }
    else
    {
        elapsedSec = (float)(travelInterval - (alignedElapsedMs - travelInterval)) / 1000.f;
    }

    m_element->setPosition({
        m_center.X + m_radius * std::sin(m_startAngleRad + m_angleSpeedRad * elapsedSec),
        m_center.Y + m_radius * std::cos(m_startAngleRad + m_angleSpeedRad * elapsedSec)
        });
}
