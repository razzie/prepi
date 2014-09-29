#include "Globals.h"
#include "level\Level.h"
#include "effects\AppearEffect.h"
#include "elements\Element.h"

using namespace irr;

AppearEffect::AppearEffect(Element* element, uint32_t duration)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_duration(duration)
 , m_pos(element->getPosition())
{
}

AppearEffect::~AppearEffect()
{
}

void AppearEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0)
    {
        m_element->enable(false); // called first time
    }
    else if ((m_elapsed + elapsedMs) >= m_duration)
    {
        m_element->enable(true); // called last time
    }

    core::vector2di pos = m_level->getScreenPosition(m_pos);
    float fade = (float)m_elapsed / (float)m_duration;
    if (fade > 1.f) fade = 1.f;

    drawTile(pos, m_origScale, 0.f, fade * 255);

    m_elapsed += elapsedMs;
}

bool AppearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
