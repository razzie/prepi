#include "Globals.h"
#include "level\Level.h"
#include "effects\DisappearEffect.h"
#include "elements\Element.h"

using namespace irr;

DisappearEffect::DisappearEffect(Element* element, uint32_t duration)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_duration(duration)
 , m_pos(element->getPosition())
{
}

DisappearEffect::~DisappearEffect()
{
}

void DisappearEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0) m_element->enable(false); // called first time

    core::vector2di pos = m_level->getScreenPosition(m_pos);
    float fade = 1.f - ((float)m_elapsed / (float)m_duration);
    if (fade < 0.f) fade = 0.f;

    drawTile(pos, m_origScale, 0.f, fade * 255);

    m_elapsed += elapsedMs;
}

bool DisappearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
