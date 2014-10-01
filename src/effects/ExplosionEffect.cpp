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
    const int angle = 90;
    unsigned particles = ((10.f - (m_scale * 2)) < 0) ? 1 : (unsigned)(10.f - (m_scale * 2));

    for (int deg = -angle; deg <= angle; deg += particles)
    {
        float intensity = m_scale + (rand() % (unsigned)(m_scale + 1.f));
        float brightness = (float)(rand() % 10) / 10.f;
        video::SColor particleColor(255,
                        m_color.getRed() * brightness,
                        m_color.getGreen() * brightness,
                        m_color.getBlue() * brightness);

        float rad = (float)deg / (2 * PI);

        core::vector2df pos = m_pos;
        pos.X += std::cos(rad) * 0.2f;
        pos.Y += -std::sin(rad) * 0.2f;

        Element* particle = new ParticleElement(m_level, m_image, pos, m_pSize, particleColor, 1000 + rand()%500);
        particle->setMovementX( std::cos(rad) * intensity );
        particle->setMovementY( -std::sin(rad) * intensity );
    }
}

bool ExplosionEffect::isFinished() const
{
    return true;
}
