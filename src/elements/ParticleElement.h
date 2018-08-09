/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef PARTICLEELEMENT_H_INCLUDED
#define PARTICLEELEMENT_H_INCLUDED

#include "irrlicht.h"
#include "Element.h"

class TileData;

class ParticleElement : public Element
{
public:
    ParticleElement(Level*, unsigned image, irr::core::vector2df pos, float scale,
                    irr::video::SColor = {255, 255, 255, 255}, unsigned life = 5000);
    ~ParticleElement();
    irr::video::SColor getColor() const;
    void setColor(irr::video::SColor);
    virtual void update(uint32_t elapsedMs);
    virtual void draw();

private:
    unsigned m_image;
    irr::video::SColor m_color;
    unsigned m_life;
    uint32_t m_elapsed;
};

#endif // PARTICLEELEMENT_H_INCLUDED
