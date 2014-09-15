#include <string>
#include <sstream>
#include "Parser.h"
#include "motions\StraightMotion.h"
#include "elements\Element.h"

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

StraightMotion::StraightMotion(Element* element, std::istream& stream)
 : StraightMotion(element, Parser(stream, ',').getArgs<uint32_t, uint32_t, bool, PointArray>())
{
}

StraightMotion::StraightMotion(Element* element, std::tuple<uint32_t, uint32_t, bool, PointArray> data)
 : StraightMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

StraightMotion::StraightMotion(Element* element, uint32_t speed, uint32_t delay, bool ai, PointArray pointArray)
 : ActiveMotion(element, Type::STRAIGHT, speed, delay, ai)
 , m_pointArray(pointArray)
{
}

StraightMotion::~StraightMotion()
{
}

StraightMotion::PointArray& StraightMotion::getPointArray()
{
    return m_pointArray;
}

const StraightMotion::PointArray& StraightMotion::getPointArray() const
{
    return m_pointArray;
}

void StraightMotion::setElement(Element* element)
{
    Motion::setElement(element);

    m_pointArray.insert(m_pointArray.begin(), element->getPosition()); // push to front
    rebuildPathArray();

    m_circularMode = (m_pointArray.front() == m_pointArray.back());
}

void StraightMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    if (m_elapsed <= m_delay) return;

    uint32_t travelInterval = m_pathArray[m_pathArray.size()-1].endTime;
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
        if (path.startTime <= alignedElapsedMs &&
            path.endTime > alignedElapsedMs)
        {
            m_element->setPosition(path.getPointByTime(alignedElapsedMs));
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

        p.startPoint = m_pointArray[i];
        p.endPoint = m_pointArray[i+1];
        float dist = p.startPoint.getDistanceFrom(p.endPoint); // distance in units

        p.startTime = ((i == 0) ? 0 : (m_pathArray[i-1].endTime));
        p.endTime = p.startTime + (uint32_t)((1000.f * dist) / (float)m_speed); // x speed: x units / 1000 msec

        m_pathArray.push_back(p);
    }
}

core::vector2df StraightMotion::Path::getPointByTime(uint32_t elapsedMs) const
{
    if (elapsedMs <= startTime)
        return startPoint;
    else if (elapsedMs >= endTime)
        return endPoint;

    uint32_t duration = endTime - startTime;
    uint32_t elapsed = elapsedMs - startTime;

    core::vector2df pathMovement = endPoint - startPoint;

    core::vector2df midPoint = startPoint;
    midPoint += (pathMovement * ((float)elapsed / (float)duration));

    return midPoint;
}

std::istream& operator>> (std::istream& stream, StraightMotion::PointArray& pointArray)
{
    Parser p(stream, ',');

    //while (p.hasNextArg())
    for (unsigned i = 0, num = p.getArg<unsigned>(); i < num; ++i)
    {
        core::vector2df point = p.getArg<core::vector2df>();
        pointArray.push_back(point);
    }

    return stream;
}
