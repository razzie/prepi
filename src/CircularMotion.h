#ifndef CIRCULARMOTION_H_INCLUDED
#define CIRCULARMOTION_H_INCLUDED

#include "irrlicht.h"
#include "ActiveMotion.h"

class CircularMotion : public ActiveMotion
{
public:
    CircularMotion(Element*, std::istream&);
    CircularMotion(Element*, std::tuple<uint32_t, uint32_t, bool, int, irr::core::vector2df>);
    CircularMotion(Element*, uint32_t speed, uint32_t delay, bool ai, int angle, irr::core::vector2df center);
    ~CircularMotion();
    irr::core::vector2df getCenter() const;
    int getAngle() const;
    void setElement(Element*);
    void update(uint32_t elapsedMs);

private:
    int m_angle;
    float m_radius;
    float m_angleSpeedRad;
    float m_startAngleRad;
    irr::core::vector2df m_center;
};

#endif // CIRCULARMOTION_H_INCLUDED
