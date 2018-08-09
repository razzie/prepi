/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
    this->reset();
}

Timer::~Timer()
{
}

Timer::Timer(const Timer& t)
 : m_startTime(t.m_startTime)
 , m_lastElapsed(t.m_lastElapsed)
{
}

uint32_t Timer::getElapsed()
{
    uint32_t elapsedFromStart =
        duration_cast<milliseconds>(steady_clock::now() - m_startTime).count();

    uint32_t elapsedNow = elapsedFromStart - m_lastElapsed;

    m_lastElapsed = elapsedFromStart;

    return elapsedNow;
}

uint32_t Timer::peekElapsed() const
{
    uint32_t elapsedFromStart =
        duration_cast<milliseconds>(steady_clock::now() - m_startTime).count();

    return elapsedFromStart - m_lastElapsed;
}

void Timer::reset()
{
    m_startTime = steady_clock::now();
    m_lastElapsed = 0;
}
