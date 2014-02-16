#ifndef REWARDELEMENT_H_INCLUDED
#define REWARDELEMENT_H_INCLUDED

#include "Element.h"
#include "Motion.h"

class RewardElement : public Element
{
public:
    RewardElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, Motion, unsigned value);
    ~RewardElement();

    Motion getMotion() const;
    unsigned getValue() const;

    void update();

private:
    Motion m_motion;
    unsigned m_value;
};

#endif // REWARDELEMENT_H_INCLUDED
