/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef ELEMENTEFFECT_H_INCLUDED
#define ELEMENTEFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\Effect.h"

class Level;
class Element;
class TileData;

// abstract parent class from element related effects
class ElementEffect : public Effect
{
public:
    ElementEffect(Element*);

protected:
    Level* m_level;
    Element* m_element;
    irr::core::vector2di m_imgPos;
    const TileData* m_tileData;
    float m_origScale;

    void drawTile(irr::core::vector2di screenPos, float scale = 1.0f,
                  float angle = 0.f, uint8_t alpha = 255) const;
};

#endif // ELEMENTEFFECT_H_INCLUDED
