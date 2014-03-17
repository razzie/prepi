#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <cstdint>
#include <chrono>

class Timer
{
    std::chrono::steady_clock::time_point m_startTime;
    uint32_t m_lastElapsed;

public:
    Timer();
    Timer(const Timer&);
    ~Timer();
    uint32_t getElapsed();
    uint32_t peekElapsed() const;
    void reset();
};

#endif // TIMER_H_INCLUDED
