/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <cmath>
#include "level\Level.h"
#include "effects\ExplosionEffect.h"
#include "elements\ParticleElement.h"

#define PI 3.14159265358979323846

using namespace irr;

ExplosionEffect::ExplosionEffect(Level* level, irr::core::vector2df pos, unsigned image, float scale, float particleSize, irr::video::SColor color)
 : m_level(level)
 , m_pos(pos)
 , m_image(image)
 , m_scale(scale)
 , m_pSize(particleSize)
 , m_color(color)
{
}

ExplosionEffect::~ExplosionEffect()
{
}

void ExplosionEffect::update(uint32_t/* elapsedMs*/)
{
    const float deg2rad = 180.f / PI;
    const int angle = 60;
    int step = 10.f - (m_scale * 2);
    if (step < 1) step = 1;

    for (int deg = -angle; deg <= angle; deg += step)
    {
        unsigned rnd = rand();
        float intensity = m_scale + (rnd % (unsigned)(m_scale + 1.f));
        float brightness = (float)(rnd % 11) / 10.f;
        video::SColor particleColor(255,
            brightness * m_color.getRed(),
            brightness * m_color.getGreen(),
            brightness * m_color.getBlue());

        float rad = deg2rad * deg;

        core::vector2df pos = m_pos;
        pos.X += std::sin(rad) * 0.5f;
        pos.Y += -std::cos(rad) * 0.5f;

        Element* particle = new ParticleElement(m_level, m_image, pos, m_pSize, particleColor, 1000 + (rnd % 500));
        particle->setMovementX( std::sin(rad) * intensity );
        particle->setMovementY( -std::cos(rad) * intensity );
    }
}

bool ExplosionEffect::isFinished() const
{
    return true;
}
