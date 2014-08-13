#include "Globals.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\Element.h"
#include "effects\ElementEffect.h"

using namespace irr;

ElementEffect::ElementEffect(Element* element)
 : m_element(element)
{
}

void ElementEffect::drawTile(irr::core::vector2di screenPos, float angle, uint8_t alpha) const
{
    if (m_element == nullptr ||
        m_element->getTileData() == nullptr)
    {
        return;
    }

    Level* level = m_element->getLevel();
    const TileData* tileData = m_element->getTileData();

    core::vector2di imgPos = m_element->getImagePosition();
    float scale = m_element->getScale();
    unsigned unit = level->getUnitSize();
    unsigned tileSize = tileData->tileSize;

    core::rect<s32> srcRect =
        {(s32)(imgPos.X * tileSize), (s32)(imgPos.Y * tileSize),
        (s32)((imgPos.X + 1) * tileSize), (s32)((imgPos.Y + 1) * tileSize)};

    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += screenPos;

    video::IVideoDriver* driver = level->getGlobals()->driver;
    driver->draw2DImage(tileData->texture, destRect, srcRect, 0, 0, true);
}

core::vector2di ElementEffect::getScreenPosition() const
{
    if (m_element == nullptr)
    {
        return core::vector2di(0, 0);
    }

    Level* level = m_element->getLevel();

    unsigned unit = level->getUnitSize();
    core::vector2df pos = m_element->getPosition();

    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};
    calcPos -= level->getViewOffset();

    return calcPos;
}
