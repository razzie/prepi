#ifndef STRAIGHTMOTION_H_INCLUDED
#define STRAIGHTMOTION_H_INCLUDED

#include <vector>
#include "irrlicht.h"
#include "ActiveMotion.h"

class StraightMotion : public ActiveMotion
{
public:
    typedef std::vector<irr::core::vector2df> PointVector;

    StraightMotion(Element*, std::istream&);
    StraightMotion(Element*, std::tuple<uint32_t, uint32_t, bool, PointVector>);
    StraightMotion(Element*, uint32_t speed, uint32_t delay, bool ai, PointVector);
    ~StraightMotion();
    PointVector& getPointVector();
    const PointVector& getPointVector() const;
    void update(uint32_t elapsedMs);

private:
    PointVector m_pointVector;
};

std::istream& operator>> (std::istream&, StraightMotion::PointVector&);

#endif // STRAIGHTMOTION_H_INCLUDED
