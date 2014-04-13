#include "Parser.h"
#include "CircularMotion.h"

using namespace irr;

std::istream& operator>> (std::istream&, irr::core::vector2df&);

CircularMotion::CircularMotion(Element* element, std::istream& stream)
 : CircularMotion(element, Parser(stream, ';').getArgs<uint32_t, uint32_t, bool, core::vector2df, int>())
{
}

CircularMotion::CircularMotion(Element* element, std::tuple<uint32_t, uint32_t, bool, core::vector2df, int> data)
 : CircularMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

CircularMotion::CircularMotion(Element* element, uint32_t speed, uint32_t delay, bool ai, core::vector2df center, int angle)
 : ActiveMotion(element, Type::STRAIGHT, speed, delay, ai)
 , m_center(center)
 , m_angle(angle)
{
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

void CircularMotion::update(uint32_t elapsedMs)
{
    //TBD
}
