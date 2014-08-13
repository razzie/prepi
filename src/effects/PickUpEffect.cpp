#include "Globals.h"
#include "level\Level.h"
#include "effects\PickUpEffect.h"

#define DURATION 250

using namespace irr;

PickUpEffect::PickUpEffect(Element* element)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_beginPos(m_level->getScreenPosition(element))
 , m_endPos(0, 0)
{
}

PickUpEffect::~PickUpEffect()
{
}

void PickUpEffect::update(uint32_t elapsedMs)
{
    core::vector2di diffPos = m_beginPos - m_endPos;
    float progress = 1.f - ((float)m_elapsed / (float)DURATION);

    diffPos.X *= progress;
    diffPos.Y *= progress;

    drawTile(diffPos);

    m_elapsed += elapsedMs;
}

bool PickUpEffect::isFinished() const
{
    return (m_elapsed >= DURATION);
}
