/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <ctime>
#include <cmath>
#include "Globals.h"
#include "level\Level.h"
#include "effects\SmokeEffect.h"

#define SMOKE_CLOUDS 5
#define DURATION 500

using namespace irr;

video::ITexture* SmokeEffect::m_texture = nullptr;

SmokeEffect::SmokeEffect(Level* level, irr::core::vector2df pos, float scale)
 : m_level(level)
 , m_pos(pos)
 , m_scale(scale)
 , m_elapsed(0)
{
    if (m_texture == nullptr)
    {
        video::IVideoDriver* driver = m_level->getGlobals()->driver;
        m_texture = driver->getTexture("../media/smoke.png");
    }

    for (int i = 0; i < SMOKE_CLOUDS; ++i)
    {
        Smoke smoke;
        smoke.m_position = pos;
        smoke.m_velocity.X = ((float)(rand() % 10) / 10.f - 0.5f) * m_scale;
        smoke.m_velocity.Y = ((float)(rand() % 10) / 10.f - 1.5f) * m_scale;
        smoke.m_color = irr::video::SColor(255, 255, 255, 255);
        smoke.m_begin = (rand() % 200);
        smoke.m_duration = DURATION - 200 + (rand() % 200);
        m_smokes.push_back(smoke);
    }
}

SmokeEffect::~SmokeEffect()
{
}

void SmokeEffect::update(uint32_t elapsedMs)
{
    Globals* g = m_level->getGlobals();
    core::recti screen({0,0}, g->driver->getScreenSize());

    for (auto& smoke : m_smokes)
    {
        // update parameters only when the smoke is visible
        if (m_elapsed >= smoke.m_begin && m_elapsed <= smoke.m_duration)
        {
            // update smoke cloud position
            float speed = (float)elapsedMs / 1000.f;
            smoke.m_position += smoke.m_velocity * speed;

            // update alpha in color
            if (m_elapsed < (smoke.m_begin + 155))
            {
                smoke.m_color.setAlpha(m_elapsed - smoke.m_begin);
            }
            else if (m_elapsed > (smoke.m_duration - 155))
            {
                smoke.m_color.setAlpha(smoke.m_duration - m_elapsed);
            }

            // calculations for drawing
            unsigned scale = (m_elapsed - smoke.m_begin) / 5 * m_scale;
            core::vector2di screenPos = m_level->getScreenPosition(smoke.m_position);
            core::recti box(-scale, -scale, scale, scale);
            box += screenPos;

            // drawing smoke
            if (screen.isPointInside(screenPos))
            {
                g->drawImage(m_texture, core::recti({0, 0}, m_texture->getSize()), box, (m_elapsed - smoke.m_begin) / 8, smoke.m_color);
            }
        }
    }

    m_elapsed += elapsedMs;
}

bool SmokeEffect::isFinished() const
{
    return (m_elapsed >= DURATION);
}
