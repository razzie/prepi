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

    // TBD
}

void StraightMotion::update(uint32_t elapsedMs)
{
    //TBD
}

std::istream& operator>> (std::istream& stream, StraightMotion::PointVector& pointVector)
{
    std::string str;
    stream >> str;
    std::stringstream ss(str);

    std::cout << "PointVector debug: " << str << std::endl;

    Parser p(ss, ';');
    while (ss) pointVector.push_back(p.getArg<core::vector2df>());

    return stream;
}
