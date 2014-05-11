#include <string>
#include <sstream>
#include "Parser.h"
#include "StraightMotion.h"
#include "Element.h"

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

StraightMotion::StraightMotion(Element* element, std::istream& stream)
 : StraightMotion(element, Parser(stream, ',').getArgs<uint32_t, uint32_t, bool, PointVector>())
{
}

StraightMotion::StraightMotion(Element* element, std::tuple<uint32_t, uint32_t, bool, PointVector> data)
 : StraightMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

StraightMotion::StraightMotion(Element* element, uint32_t speed, uint32_t delay, bool ai, PointVector pointVector)
 : ActiveMotion(element, Type::STRAIGHT, speed, delay, ai)
 , m_pointVector(pointVector)
{
}

StraightMotion::~StraightMotion()
{
}

StraightMotion::PointVector& StraightMotion::getPointVector()
{
    return m_pointVector;
}

const StraightMotion::PointVector& StraightMotion::getPointVector() const
{
    return m_pointVector;
}

void StraightMotion::setElement(Element* element)
{
    Motion::setElement(element);

    m_pointVector.insert(m_pointVector.begin(), element->getPosition()); // push to front
    rebuildPathVector();

    m_circularMode = (m_pointVector[0] == m_pointVector[m_pointVector.size()-1]);
}

void StraightMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    if (m_elapsed <= m_delay) return;

    uint32_t travelInterval = m_pathVector[m_pathVector.size()-1].endTime;
    uint32_t alignedElapsedMs = (m_elapsed - m_delay) % (travelInterval * 2);

    for (auto& path : m_pathVector)
    {
        if (path.startTime >= alignedElapsedMs)
        {
            m_element->setPosition(path.getPointByTime(alignedElapsedMs));
        }
    }
}

void StraightMotion::rebuildPathVector()
{
    m_pathVector.clear();

    for (unsigned i = 0; i < m_pathVector.size()-1; ++i)
    {
        Path p;

        p.startPoint = m_pointVector[i];
        p.endPoint = m_pointVector[i+1];
        float dist = p.startPoint.getDistanceFrom(p.endPoint); // distance in units

        p.startTime = (i == 0) ? 0 : m_pathVector[i-1].endTime;
        p.endTime = p.startTime + ((100 * dist) / m_speed); // 100 means: 10 units / 1000 msec
    }
}

core::vector2df StraightMotion::Path::getPointByTime(uint32_t elapsedMs) const
{
    if (elapsedMs < startTime)
        return startPoint;
    else if (elapsedMs > endTime)
        return endPoint;

    uint32_t duration = endTime - startTime;
    uint32_t elapsed = elapsedMs - startTime;

    core::vector2df pathMovement = endPoint - startPoint;

    core::vector2df midPoint = startPoint;
    midPoint += (pathMovement / (elapsed/duration));

    return midPoint;
}

std::istream& operator>> (std::istream& stream, StraightMotion::PointVector& pointVector)
{
    stream.ignore(); // ignore colon

    std::string str;
    std::getline(stream, str);
    std::stringstream ss(str);

    //std::cout << "PointVector debug: " << str << std::endl;

    Parser p(ss, ',');
    while (ss)
    {
        core::vector2df point = p.getArg<core::vector2df>();
        pointVector.push_back(point);
        std::cout << "x: " << point.X << ", y: " << point.Y << std::endl;
    }

    return stream;
}
