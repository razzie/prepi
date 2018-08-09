/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <stdexcept>
#include "Parser.h"
#include "motions\AutoMotion.h"
#include "elements\Element.h"

using namespace irr;

AutoMotion::AutoMotion(Element* element, std::istream& stream)
 : AutoMotion(element, Parser(stream, ',').getArgs<float, uint32_t, bool, Direction>())
{
}

AutoMotion::AutoMotion(Element* element, std::tuple<float, uint32_t, bool, Direction> data)
 : AutoMotion(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

AutoMotion::AutoMotion(Element* element, float speed, uint32_t delay, bool ai, Direction direction)
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
        Element* contactElem = collision.getElementB();

        if (contactElem->getType() == Element::Type::PARTICLE) continue;

        switch (collision.getDirection(45.f))
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
    }

    if (isOnGround)
    {
        if (m_direction == Direction::LEFT)
            m_element->setMovementX(-m_speed);
        else
            m_element->setMovementX(m_speed);
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
