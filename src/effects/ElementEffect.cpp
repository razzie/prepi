#include "Globals.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\Element.h"
#include "effects\ElementEffect.h"

using namespace irr;

ElementEffect::ElementEffect(Element* element)
 : m_level(element ? element->getLevel() : nullptr)
 , m_element(element)
 , m_imgPos(element ? element->getImagePosition() : core::vector2di(0,0))
 , m_tileData(element ? element->getTileData() : nullptr)
 , m_origScale(element ? element->getScale() : 1.f)
{
}

void ElementEffect::drawTile(irr::core::vector2di screenPos, float scale, float angle, uint8_t alpha) const
{
    if (m_tileData == nullptr) return;

    const unsigned unit = m_level->getUnitSize();
    const unsigned tileSize = m_tileData->getTileSize();

    core::rect<s32> srcRect =
        {(s32)(m_imgPos.X * tileSize), (s32)(m_imgPos.Y * tileSize),
        (s32)((m_imgPos.X + 1) * tileSize), (s32)((m_imgPos.Y + 1) * tileSize)};

    core::rect<s32> destRect = {0, 0, (s32)(scale * m_origScale * unit), (s32)(scale * m_origScale * unit)};
    destRect += screenPos;

    const video::SColor color(alpha, 255, 255, 255);
	const video::SColor tempColors[4] = {color, color, color, color};

    video::IVideoDriver* driver = m_level->getGlobals()->driver;
    driver->draw2DImage(m_tileData->getTexture(), destRect, srcRect, 0, tempColors, true);
}
