#ifndef ACTIVEMOTION_H_INCLUDED
#define ACTIVEMOTION_H_INCLUDED

#include <cstdint>
#include <tuple>
#include "Motion.h"

class ActiveMotion : public Motion
{
public:
    virtual ~ActiveMotion();
    uint32_t getSpeed() const;
    uint32_t getDelay() const;
    bool getAI() const;
    virtual void update(uint32_t elapsedMs);

protected:
    //ActiveMotion(Element*, Motion::Type, std::istream&);
    //ActiveMotion(Element*, Motion::Type, std::tuple<uint32_t, uint32_t, bool>);
    ActiveMotion(Element*, Motion::Type, uint32_t speed, uint32_t delay, bool ai);

    uint32_t m_elapsed;
    uint32_t m_speed;
    uint32_t m_delay;
    bool m_ai;
};

#endif // ACTIVEMOTION_H_INCLUDED
