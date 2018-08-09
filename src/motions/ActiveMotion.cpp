/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Parser.h"
#include "motions\ActiveMotion.h"
#include "elements\Element.h"

/*ActiveMotion::ActiveMotion(Element* element, Motion::Type type, std::istream& stream)
 : ActiveMotion(element, type, Parser(stream, ';').getArgs<uint32_t, uint32_t, bool>())
{
}*/

/*ActiveMotion::ActiveMotion(Element* element, Motion::Type type, std::tuple<float, uint32_t, bool> data)
 : ActiveMotion(element, type,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}*/

ActiveMotion::ActiveMotion(Element* element, Motion::Type type, float speed, uint32_t delay, bool ai)
 : Motion(element, type)
 , m_elapsed(0)
 , m_speed(speed)
 , m_delay(delay)
 , m_ai(ai)
{
}

ActiveMotion::~ActiveMotion()
{
}

float ActiveMotion::getSpeed() const
{
    return m_speed;
}

uint32_t ActiveMotion::getDelay() const
{
    return m_delay;
}

bool ActiveMotion::getAI() const
{
    return m_ai;
}

void ActiveMotion::update(uint32_t)
{
}
