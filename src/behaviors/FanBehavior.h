#ifndef FANBEHAVIOR_H_INCLUDED
#define FANBEHAVIOR_H_INCLUDED

#include <tuple>
#include "irrlicht.h"
#include "behaviors\Behavior.h"

class Element;

class FanBehavior : public Behavior
{
public:
    FanBehavior(Element*, std::istream&);
    FanBehavior(Element*, std::tuple<irr::core::vector2df, bool>);
    FanBehavior(Element*, irr::core::vector2df velocity, bool reverse);
    ~FanBehavior();
    void update(uint32_t elapsedMs);

protected:
    irr::core::vector2df m_velocity;
    irr::core::vector2df m_points[4];
    bool m_reverse;
};

#endif // FANBEHAVIOR_H_INCLUDED
