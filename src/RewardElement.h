#ifndef REWARDELEMENT_H_INCLUDED
#define REWARDELEMENT_H_INCLUDED

#include "Element.h"

class RewardElement : public Element
{
public:
    RewardElement(Level*, std::istream&);
    RewardElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, unsigned, BehaviorWrapper, MotionWrapper>);
    RewardElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, float, unsigned value, Behavior*, Motion*);
    unsigned getValue() const;

    //void update(uint32_t elapsedMs);
    //void draw();

private:
    ~RewardElement();

    unsigned m_value;
};

#endif // REWARDELEMENT_H_INCLUDED
