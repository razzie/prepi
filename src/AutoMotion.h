#ifndef AUTOMOTION_H_INCLUDED
#define AUTOMOTION_H_INCLUDED

#include "ActiveMotion.h"

class AutoMotion : public ActiveMotion
{
public:
    enum Direction
    {
        LEFT  = 0,
        RIGHT = 1
    };

    AutoMotion(Element*, std::istream&);
    AutoMotion(Element*, std::tuple<uint32_t, uint32_t, bool, Direction>);
    AutoMotion(Element*, uint32_t speed, uint32_t delay, bool ai, Direction);
    ~AutoMotion();
    Direction getDirection() const;
    void setElement(Element*);
    void update(uint32_t elapsedMs);

private:
    Direction m_direction;
};

std::istream& operator>> (std::istream&, AutoMotion::Direction&);

#endif // AUTOMOTION_H_INCLUDED
