#include "Globals.h"
#include "level\Level.h"
#include "effects\AppearEffect.h"
#include "elements\Element.h"

using namespace irr;

AppearEffect::AppearEffect(Element* element, uint32_t duration)
 : ElementEffect(element)
 , m_elapsed(0)
 , m_duration(duration)
 , m_pos(m_level->getScreenPosition(element))
{
    //m_element->enable(false);
}

AppearEffect::~AppearEffect()
{
    m_element->enable(true);
}

void AppearEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0) m_element->enable(false); // called first time

    float fade = (float)m_elapsed / (float)m_duration;
    if (fade > 1.f) fade = 1.f;

    drawTile(m_pos, m_origScale, 0.f, fade * 255);

    m_elapsed += elapsedMs;
}

bool AppearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
