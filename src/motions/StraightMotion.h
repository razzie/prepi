#ifndef STRAIGHTMOTION_H_INCLUDED
#define STRAIGHTMOTION_H_INCLUDED

#include <vector>
#include "irrlicht.h"
#include "Shape.h" // for PointArray
#include "ActiveMotion.h"

class StraightMotion : public ActiveMotion
{
public:
    StraightMotion(Element*, std::istream&);
    StraightMotion(Element*, std::tuple<uint32_t, uint32_t, bool, PointArray>);
    StraightMotion(Element*, uint32_t speed, uint32_t delay, bool ai, PointArray);
    ~StraightMotion();
    PointArray& getPointArray();
    const PointArray& getPointArray() const;
    void setElement(Element*);
    void update(uint32_t elapsedMs);

private:
    struct Path
    {
        irr::core::vector2df startPoint;
        irr::core::vector2df endPoint;
        uint32_t startTime;
        uint32_t endTime;

        irr::core::vector2df getPointByTime(uint32_t) const;
    };

    void rebuildPathArray();

    PointArray m_pointArray;
    std::vector<Path> m_pathArray;
    bool m_circularMode;
};

#endif // STRAIGHTMOTION_H_INCLUDED
