#include <stdexcept>
#include "Parser.h"
#include "motions\AutoMotion.h"
#include "elements\Element.h"

using namespace irr;

AutoMotion::AutoMotion(Element* element, std::istream& stream)
 : AutoMotion(element, Parser(stream, ',').getArgs<uint32_t, uint32_t, bool, Direction>())
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

void AutoMotion::setElement(Element* element)
{
    Motion::setElement(element);

    // TBD
}

void AutoMotion::update(uint32_t elapsedMs)
{
    m_elapsed += elapsedMs;

    if (m_elapsed <= m_delay) return;

    bool isOnGround = false;

    m_element->updateCollisions();
    for (const Collision& collision : m_element->getCollisions())
    {
        Collision::Direction direction = Collision::getDirectionFromAngle( collision.getOtherElementAngle(), 45.f );
        switch (direction)
        {
            case Collision::Direction::LEFT:
                m_direction = Direction::RIGHT;
                break;

            case Collision::Direction::RIGHT:
                m_direction = Direction::LEFT;
                break;

            case Collision::Direction::BOTTOM:
                isOnGround = true;
                break;

            default:
                break;
        }

        collision.getOtherElement()->drawDebugBox();
    }

    if (isOnGround)
    {
        float speed = (float)m_speed / 10.f;

        if (m_direction == Direction::LEFT)
            m_element->setMovementX(-speed);
        else
            m_element->setMovementX(speed);
    }
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
