#include "Globals.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "effects\AppearEffect.h"
#include "elements\Element.h"

using namespace irr;

AppearEffect::AppearEffect(Element* element)
 : ElementEffect(element)
 , m_pos(element->getPosition())
 , m_elapsed(0)
 , m_duration(250)
 , m_animation(false)
{
    const TileData::Animation* anim;

    if (m_tileData != nullptr &&
        (anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::APPEAR)) != nullptr)
    {
        m_animation = true;
        m_duration = (anim->m_frames * 1000) / anim->m_speed;
    }
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

    if (m_animation)
    {
        const TileData::Animation* anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::APPEAR);
        unsigned frame = (m_elapsed * anim->m_speed) / 1000;

        m_tileData->drawAnimation(m_level, m_imgPos, TileData::Animation::Type::APPEAR, 0.f, m_pos, m_origScale,
                                  0.f, {255, 255, 255, 255}, frame);
    }
    else
    {
        core::vector2di pos = m_level->getScreenPosition(m_pos);
        float fade = (float)m_elapsed / (float)m_duration;
        if (fade > 1.f) fade = 1.f;

        drawTile(pos, m_origScale, 0.f, fade * 255);
    }

    m_elapsed += elapsedMs;
}

bool AppearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
