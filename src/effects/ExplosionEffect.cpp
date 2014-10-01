#include "level\Level.h"
#include "effects\ExplosionEffect.h"
#include "elements\ParticleElement.h"

#define PI 3.14159265358979323846

using namespace irr;

ExplosionEffect::ExplosionEffect(Level* level, irr::core::vector2df pos, float range, irr::video::SColor color)
 : m_level(level)
 , m_pos(pos)
 , m_range(range)
 , m_color(color)
{
}

ExplosionEffect::~ExplosionEffect()
{
}

void ExplosionEffect::update(uint32_t/* elapsedMs*/)
{
    const int angle = 90;
    unsigned particles = ((10.f - (m_range * 2)) < 0) ? 1 : (unsigned)(10.f - (m_range * 2));

    for (int deg = -angle; deg <= angle; deg += particles)
    {
        float intensity = 4.f + m_range + (rand() % (unsigned)(m_range + 0.5f));
        float brightness = (float)(rand() % 10) / 10.f;
        video::SColor particleColor(255,
                        m_color.getRed() * brightness,
                        m_color.getGreen() * brightness,
                        m_color.getBlue() * brightness);

        Element* particle = new ParticleElement(m_level, 0, m_pos, 0.08f, particleColor, 1000 + rand()%500);
        particle->setMovementX( sin((float)deg / (2 * PI)) * intensity );
        particle->setMovementY( cos((float)deg / (2 * PI)) * intensity );
    }
}

bool ExplosionEffect::isFinished() const
{
    return true;
}
