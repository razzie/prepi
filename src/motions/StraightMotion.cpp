#include <string>
#include <sstream>
#include "Box2D\Box2D.h"
#include "Parser.h"
#include "motions\StraightMotion.h"
#include "elements\Element.h"

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

StraightMotion::StraightMotion(Element* element, std::istream& stream)
 : StraightMotion(element, Parser(stream, ',').getArgs<float, uint32_t, bool, PointArray>())
{
}

StraightMotion::StraightMotion(Element* element, std::tuple<float, uint32_t, bool, PointArray> data)
 : StraightMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

StraightMotion::StraightMotion(Element* element, float speed, uint32_t delay, bool ai, PointArray pointArray)
 : ActiveMotion(element, Type::STRAIGHT, speed, delay, ai)
 , m_pointArray(pointArray)
{
}

StraightMotion::~StraightMotion()
{
}

PointArray& StraightMotion::getPointArray()
{
    return m_pointArray;
}

const PointArray& StraightMotion::getPointArray() const
{
    return m_pointArray;
}

void StraightMotion::setElement(Element* element)
{
    Motion::setElement(element);

    for (auto& p : m_pointArray) p -= core::vector2df(0.5f, 0.5f); // element coordinate is at the upper left corner, not at center
    m_pointArray.insert(m_pointArray.begin(), element->getPosition()); // push player position to front
    rebuildPathArray();

    m_circularMode = (m_pointArray.front() == m_pointArray.back());
}

void StraightMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    if (m_elapsed <= m_delay || m_element == nullptr) return;

    uint32_t travelInterval = m_pathArray[m_pathArray.size()-1].m_endTime;
    uint32_t alignedElapsedMs;

    if (m_circularMode)
    {
        alignedElapsedMs = (m_elapsed - m_delay) % travelInterval;
    }
    else
    {
        alignedElapsedMs = (m_elapsed - m_delay) % (travelInterval * 2);

        if (alignedElapsedMs > travelInterval)
            alignedElapsedMs = travelInterval - (alignedElapsedMs - travelInterval);
    }

    for (auto& path : m_pathArray)
    {
        if (path.m_startTime <= alignedElapsedMs &&
            path.m_endTime > alignedElapsedMs)
        {
            core::vector2df pos = path.getPointByTime(alignedElapsedMs);
            //core::vector2df velocity = pos - m_element->getPosition();

            m_element->setPosition(pos);
            m_element->setMovementX(path.m_velocity.X);
            m_element->setMovementY(path.m_velocity.Y);
            //m_element->getBody()->SetLinearVelocity({path.m_velocity.X, path.m_velocity.Y});

            break;
        }
    }
}

void StraightMotion::rebuildPathArray()
{
    m_pathArray.clear();
    m_pathArray.reserve( m_pointArray.size() );

    if (m_pointArray.size() == 0) return;

    for (unsigned i = 0; i < m_pointArray.size()-1; ++i)
    {
        Path p;

        p.m_startPoint = m_pointArray[i];
        p.m_endPoint = m_pointArray[i+1];
        float dist = p.m_startPoint.getDistanceFrom(p.m_endPoint); // distance in units

        p.m_startTime = ((i == 0) ? 0 : (m_pathArray[i-1].m_endTime));
        p.m_endTime = p.m_startTime + (uint32_t)((1000.f * dist) / (float)m_speed); // x speed: x units / 1000 msec

        p.m_velocity = (p.m_startPoint - p.m_endPoint) / ((float)p.m_endTime - (float)p.m_startTime);

        m_pathArray.push_back(p);
    }
}

core::vector2df StraightMotion::Path::getPointByTime(uint32_t elapsedMs) const
{
    if (elapsedMs <= m_startTime)
        return m_startPoint;
    else if (elapsedMs >= m_endTime)
        return m_endPoint;

    uint32_t duration = m_endTime - m_startTime;
    uint32_t elapsed = elapsedMs - m_startTime;

    core::vector2df pathMovement = m_endPoint - m_startPoint;

    core::vector2df midPoint = m_startPoint;
    midPoint += (pathMovement * ((float)elapsed / (float)duration));

    return midPoint;
}
