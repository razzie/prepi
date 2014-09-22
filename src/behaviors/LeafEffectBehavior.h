#ifndef LEAFEFFECTBEHAVIOR_H_INCLUDED
#define LEAFEFFECTBEHAVIOR_H_INCLUDED

#include <tuple>
#include "irrlicht.h"
#include "Behavior.h"

class LeafEffectBehavior : public Behavior
{
public:
    LeafEffectBehavior(Element*, std::istream&);
    LeafEffectBehavior(Element*, std::tuple<irr::video::SColor, irr::core::vector2df>);
    LeafEffectBehavior(Element*, irr::video::SColor, irr::core::vector2df);
    ~LeafEffectBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    irr::video::SColor m_color;
    irr::core::vector2df m_direction;
    uint32_t m_genSpeed;
    uint32_t m_elapsed;
};

#endif // LEAFEFFECTBEHAVIOR_H_INCLUDED
