#ifndef LEAFEFFECTBEHAVIOR_H_INCLUDED
#define LEAFEFFECTBEHAVIOR_H_INCLUDED

#include <tuple>
#include "irrlicht.h"
#include "behaviors\Behavior.h"

class LeafEffectBehavior : public Behavior
{
public:
    LeafEffectBehavior(Element*, std::istream&);
    LeafEffectBehavior(Element*, std::tuple<unsigned, irr::video::SColor, irr::core::vector2df, float>);
    LeafEffectBehavior(Element*, unsigned, irr::video::SColor, irr::core::vector2df, float);
    ~LeafEffectBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    unsigned m_image;
    irr::video::SColor m_color;
    irr::core::vector2df m_direction;
    float m_length;
    uint32_t m_genSpeed;
    uint32_t m_elapsed;
};

#endif // LEAFEFFECTBEHAVIOR_H_INCLUDED
