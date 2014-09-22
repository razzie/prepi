#ifndef LEAFEFFECT_H_INCLUDED
#define LEAFEFFECT_H_INCLUDED

#include <vector>
#include "irrlicht.h"
#include "effects\EffectManager.h"

class Element;
class Level;

class LeafEffect : public Effect
{
public:
    LeafEffect(Element*, irr::video::SColor, irr::core::vector2df);
    ~LeafEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    struct Leaf
    {
        irr::core::vector2df m_position;
        irr::video::SColor m_color;
        uint32_t m_begin;
        uint32_t m_duration;
        uint32_t m_randomSeed;
    };

    static irr::video::ITexture* m_leafTexture;
    Level* m_level;
    irr::video::SColor m_color;
    irr::core::vector2df m_direction;
    irr::core::rectf m_box;
    uint32_t m_duration;
    uint32_t m_elapsed;
    std::vector<Leaf> m_leafs;
};

#endif // LEAFEFFECT_H_INCLUDED
