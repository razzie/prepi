#include "Globals.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "effects\DisappearEffect.h"
#include "elements\Element.h"

using namespace irr;

DisappearEffect::DisappearEffect(Element* element)
 : ElementEffect(element)
 , m_pos(element->getPosition())
 , m_elapsed(0)
 , m_duration(250)
 , m_animation(false)
{
    const TileData::Animation* anim;

    if (m_tileData != nullptr &&
        (anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::DISAPPEAR)) != nullptr)
    {
        m_animation = true;
        m_duration = (anim->m_frames * 1000) / anim->m_speed;
    }
}

DisappearEffect::~DisappearEffect()
{
}

void DisappearEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0) m_element->enable(false); // called first time

    if (m_animation)
    {
        const TileData::Animation* anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::DISAPPEAR);
        unsigned frame = (m_elapsed * anim->m_speed) / 1000;

        m_tileData->drawAnimation(m_level, m_imgPos, TileData::Animation::Type::DISAPPEAR, 0.f, m_pos, m_origScale,
                                  0.f, {255, 255, 255, 255}, frame);
    }
    else
    {
        core::vector2di pos = m_level->getScreenPosition(m_pos);
        float fade = 1.f - ((float)m_elapsed / (float)m_duration);
        if (fade < 0.f) fade = 0.f;

        drawTile(pos, m_origScale, 0.f, fade * 255);
    }

    m_elapsed += elapsedMs;
}

bool DisappearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
