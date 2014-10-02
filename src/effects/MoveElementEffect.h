#ifndef MOVEELEMENTEFFECT_H_INCLUDED
#define MOVEELEMENTEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\ElementEffect.h"

class Element;

class MoveElementEffect : public ElementEffect
{
public:
    MoveElementEffect(Element*, irr::core::vector2di absEndPos, uint32_t duration = 250);
    MoveElementEffect(Element*, irr::core::vector2df relEndPos, uint32_t duration = 250);
    ~MoveElementEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    uint32_t m_elapsed;
    uint32_t m_duration;
    irr::core::vector2di m_beginPos;
    irr::core::vector2di m_absEndPos;
    irr::core::vector2df m_relEndPos;
    bool m_absMove;
};

#endif // MOVEELEMENTEFFECT_H_INCLUDED
