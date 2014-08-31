#include "Globals.h"
#include "level\Level.h"
#include "effects\MoveElementEffect.h"

using namespace irr;

MoveElementEffect::MoveElementEffect(Element* element, irr::core::vector2di absEndPos, uint32_t duration)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_duration(duration)
 , m_beginPos(m_level->getScreenPosition(element))
 , m_absEndPos(absEndPos)
 , m_absMove(true)
{
}

MoveElementEffect::MoveElementEffect(Element* element, irr::core::vector2df relEndPos, uint32_t duration)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_duration(duration)
 , m_beginPos(m_level->getScreenPosition(element))
 , m_relEndPos(relEndPos)
 , m_absMove(false)
{
}

MoveElementEffect::~MoveElementEffect()
{
}

void MoveElementEffect::update(uint32_t elapsedMs)
{
    core::vector2di endPos = (m_absMove) ? m_absEndPos : m_level->getScreenPosition(m_relEndPos);

    core::vector2di diffPos = endPos - m_beginPos;
    float progress = (float)m_elapsed / (float)m_duration;

    diffPos.X *= progress;
    diffPos.Y *= progress;

    drawTile(m_beginPos + diffPos);

    m_elapsed += elapsedMs;
}

bool MoveElementEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
