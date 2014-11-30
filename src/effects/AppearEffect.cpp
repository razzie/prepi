#include "Globals.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "effects\AppearEffect.h"
#include "elements\Element.h"

using namespace irr;

/* ---------------------------------- APPEAR EFFECT ---------------------------------- */

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
    if (m_elapsed == 0) // called first time
    {
        m_element->setFlags(true);
        m_element->setFlag(Element::Flag::DRAW, false);
    }
    else if ((m_elapsed + elapsedMs) >= m_duration) // called last time
    {
        m_element->setFlag(Element::Flag::DRAW, true);
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

        drawTile(pos, 1.f, 0.f, fade * 255);
    }

    m_elapsed += elapsedMs;
}

bool AppearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}

/* ---------------------------------- DISAPPEAR EFFECT ---------------------------------- */

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
    if (m_elapsed == 0) m_element->setFlags(false); // called first time

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

        drawTile(pos, 1.f, 0.f, fade * 255);
    }

    m_elapsed += elapsedMs;
}

bool DisappearEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}

/* ---------------------------------- REAPPEAR EFFECT ---------------------------------- */

ReappearEffect::ReappearEffect(Element* element, core::vector2df newPos)
 : ElementEffect(element)
 , m_pos1(element->getPosition())
 , m_pos2(newPos)
 , m_elapsed(0)
 , m_duration1(250)
 , m_duration2(250)
 , m_animation1(false)
 , m_animation2(false)
{
    const TileData::Animation* anim;

    if (m_tileData != nullptr)
    {
        if ((anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::APPEAR)) != nullptr)
        {
            m_animation1 = true;
            m_duration1 = (anim->m_frames * 1000) / anim->m_speed;
        }

        if ((anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::DISAPPEAR)) != nullptr)
        {
            m_animation2 = true;
            m_duration2 = (anim->m_frames * 1000) / anim->m_speed;
        }
    }
}

ReappearEffect::~ReappearEffect()
{
}

void ReappearEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0)
        m_element->setFlags(false); // called first time
    else if (m_elapsed <= m_duration1 && (m_elapsed + elapsedMs) > m_duration1)
        m_element->setPosition(m_pos2); // move to new position
    else if ((m_elapsed + elapsedMs) >= (m_duration1 + m_duration2))
        m_element->setFlags(true); // activate

    if (m_elapsed <= m_duration1) // disappear
    {
        if (m_animation1)
        {
            const TileData::Animation* anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::DISAPPEAR);
            unsigned frame = (m_elapsed * anim->m_speed) / 1000;

            m_tileData->drawAnimation(m_level, m_imgPos, TileData::Animation::Type::DISAPPEAR, 0.f, m_pos1, m_origScale,
                                      0.f, {255, 255, 255, 255}, frame);
        }
        else
        {
            core::vector2di pos = m_level->getScreenPosition(m_pos1);
            float fade = 1.f - ((float)m_elapsed / (float)m_duration1);
            if (fade < 0.f) fade = 0.f;

            drawTile(pos, 1.f, 0.f, fade * 255);
        }
    }
    else // appear
    {
        if (m_animation2)
        {
            const TileData::Animation* anim = m_tileData->getAnimation(m_imgPos, TileData::Animation::Type::APPEAR);
            unsigned frame = ((m_elapsed - m_duration1) * anim->m_speed) / 1000;

            m_tileData->drawAnimation(m_level, m_imgPos, TileData::Animation::Type::APPEAR, 0.f, m_pos2, m_origScale,
                                      0.f, {255, 255, 255, 255}, frame);
        }
        else
        {
            core::vector2di pos = m_level->getScreenPosition(m_pos2);
            float fade = (float)(m_elapsed - m_duration1) / (float)m_duration2;
            if (fade > 1.f) fade = 1.f;

            drawTile(pos, 1.f, 0.f, fade * 255);
        }
    }

    m_elapsed += elapsedMs;
}

bool ReappearEffect::isFinished() const
{
    return (m_elapsed >= (m_duration1 + m_duration2));
}
