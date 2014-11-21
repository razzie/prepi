#include <cmath>
#include "Angle.h"

static float alignedAngle(float angle)
{
    if (angle < 0.f)
        return (360.f + fmod(angle, 360.f));
    else if (angle >= 360.f)
        return fmod(angle, 360.f);
    else
        return angle;
}

DegAngle::DegAngle(float angle)
 : m_angle(alignedAngle(angle))
{
}

DegAngle::~DegAngle()
{
}

DegAngle& DegAngle::operator=(float angle)
{
    m_angle = alignedAngle(angle);
    return *this;
}

DegAngle DegAngle::operator+(float angle) const
{
    return DegAngle(m_angle + angle);
}

DegAngle& DegAngle::operator+=(float angle)
{
    m_angle = alignedAngle(m_angle + angle);
    return *this;
}

DegAngle DegAngle::operator-(float angle) const
{
    return DegAngle(m_angle - angle);
}

DegAngle& DegAngle::operator-=(float angle)
{
    m_angle = alignedAngle(m_angle - angle);
    return *this;
}

DegAngle::operator float() const
{
    return m_angle;
}

DegAngle& DegAngle::rotateTowards(float angle, float rate)
{
    m_angle += 360.f;
    angle = alignedAngle(angle) + 360.f;

    if (angle > m_angle)
    {
        m_angle += rate;
        if (m_angle > angle) m_angle = angle;
    }
    else if (angle < m_angle)
    {
        m_angle -= rate;
        if (m_angle < angle) m_angle = angle;
    }

    m_angle = alignedAngle(m_angle);

    return *this;
}

float DegAngle::distanceFrom(float angle) const
{
    float rawDist = std::abs(alignedAngle(angle) - m_angle);
    if (rawDist > 180.f)
    {
        rawDist -= (rawDist - 180.f);
    }
    return rawDist;
}

bool DegAngle::isBetween(float minAngle, float maxAngle) const
{
    minAngle = alignedAngle(minAngle);
    maxAngle = alignedAngle(maxAngle);

    if (maxAngle >= minAngle)
    {
        return (m_angle >= minAngle && m_angle <= maxAngle);
    }
    else
    {
        return ((m_angle >= minAngle && m_angle < 360) ||
                (m_angle >= 0 && m_angle <= maxAngle));
    }
}
