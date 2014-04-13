#include <stdexcept>
#include "Parser.h"
#include "AutoMotion.h"

AutoMotion::AutoMotion(Element* element, std::istream& stream)
 : AutoMotion(element, Parser(stream, ';').getArgs<uint32_t, uint32_t, bool, Direction>())
{
}

AutoMotion::AutoMotion(Element* element, std::tuple<uint32_t, uint32_t, bool, Direction> data)
 : AutoMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

AutoMotion::AutoMotion(Element* element, uint32_t speed, uint32_t delay, bool ai, Direction direction)
 : ActiveMotion(element, Type::AUTO, speed, delay, ai)
 , m_direction(direction)
{
}

AutoMotion::~AutoMotion()
{
}

AutoMotion::Direction AutoMotion::getDirection() const
{
    return m_direction;
}

void AutoMotion::update(uint32_t elapsedMs)
{
    //TBD
}

std::istream& operator>> (std::istream& stream, AutoMotion::Direction& dir)
{
    int i;
    stream >> i;

    switch (i)
    {
        case 0:
            dir = AutoMotion::Direction::LEFT;
            break;

        case 1:
            dir = AutoMotion::Direction::RIGHT;
            break;

        default:
            throw std::runtime_error("unknown direction");
    }

    return stream;
}
