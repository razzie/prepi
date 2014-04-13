#ifndef CIRCULARMOTION_H_INCLUDED
#define CIRCULARMOTION_H_INCLUDED

#include "irrlicht.h"
#include "ActiveMotion.h"

class CircularMotion : public ActiveMotion
{
public:
    CircularMotion(Element*, std::istream&);
    CircularMotion(Element*, std::tuple<uint32_t, uint32_t, bool, irr::core::vector2df, int>);
    CircularMotion(Element*, uint32_t speed, uint32_t delay, bool ai, irr::core::vector2df center, int angle);
    ~CircularMotion();
    irr::core::vector2df getCenter() const;
    int getAngle() const;
    void update(uint32_t elapsedMs);

private:
    irr::core::vector2df m_center;
    int m_angle;
};

#endif // CIRCULARMOTION_H_INCLUDED
