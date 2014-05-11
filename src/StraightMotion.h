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

    void rebuildPathVector();

    PointVector m_pointVector;
    std::vector<Path> m_pathVector;
    unsigned m_currentPath;
    bool m_circularMode;
};

std::istream& operator>> (std::istream&, StraightMotion::PointVector&);

#endif // STRAIGHTMOTION_H_INCLUDED
