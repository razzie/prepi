/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef LEAFEFFECT_H_INCLUDED
#define LEAFEFFECT_H_INCLUDED

#include <vector>
#include "irrlicht.h"
#include "effects\Effect.h"

class Element;
class Level;
class TileData;

class LeafEffect : public Effect
{
public:
    LeafEffect(Element*, unsigned image, irr::video::SColor, irr::core::vector2df velocity, float length);
    LeafEffect(Level*, const Shape& shape, unsigned image, irr::video::SColor, irr::core::vector2df velocity, float length);
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

    Level* m_level;
    const TileData* m_tileData;
    irr::core::vector2di m_imgPos; // leaf tile
    irr::video::SColor m_color;
    irr::core::vector2df m_velocity;
    Shape m_shape;
    uint32_t m_duration;
    uint32_t m_elapsed;
    std::vector<Leaf> m_leafs;
};

#endif // LEAFEFFECT_H_INCLUDED
