#include <ctime>
#include "Globals.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "elements\ParticleElement.h"
#include "effects\EffectManager.h"

#define PI 3.14159265358979323846

using namespace irr;

EffectManager::EffectManager(Level* level)
 : m_level(level)
{
}

EffectManager::~EffectManager()
{
    for (Effect* effect : m_effects)
    {
        delete effect;
    }

    m_effects.clear();
}

void EffectManager::addEffect(Effect* effect)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_effects.push_back(effect);
}

void EffectManager::update(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    for (auto it = m_effects.begin(); it != m_effects.end(); )
    {
        Effect* effect = *it;

        effect->update(elapsedMs);

        if (effect->isFinished())
        {
            delete effect;
            it = m_effects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EffectManager::playerDamage()
{
    class DamageEffect : public Effect
    {
    private:
        Level* m_level;

    public:
        DamageEffect(Level* level) : m_level(level) {}

        void update(uint32_t)
        {
            video::IVideoDriver* driver = m_level->getGlobals()->driver;
            core::recti screen({0,0}, driver->getScreenSize());

            driver->draw2DRectangle(video::SColor(100, 255, 0, 0), screen);
        }

        bool isFinished() const
        {
            return true;
        }
    };

    // adding red screen to effects
    addEffect(new DamageEffect(m_level));

    // adding some blood
    Element* player = m_level->getPlayerElement();
    if (player)
    {
        explosion(player->getPosition() + player->getBoundingBox().getCenter(), 0.5f);
    }
}

void EffectManager::explosion(irr::core::vector2df pos, float range, irr::video::SColor color)
{
    const int angle = 90;
    unsigned particles = ((10.f - (range * 2)) < 0) ? 1 : (unsigned)(10.f - (range * 2));

    for (int deg = -angle; deg <= angle; deg += particles)
    {
        float intensity = 4.f + range + (rand() % (unsigned)(range + 0.5f));
        float brightness = (float)(rand() % 10) / 10.f;
        video::SColor particleColor(255,
                        color.getRed() * brightness,
                        color.getGreen() * brightness,
                        color.getBlue() * brightness);

        Element* particle = new ParticleElement(m_level, particleColor, pos, 1000 + rand()%500);
        particle->setMovementX( sin((float)deg / (2 * PI)) * intensity );
        particle->setMovementY( cos((float)deg / (2 * PI)) * intensity );
    }
}

void EffectManager::appear(Element* element)
{

}

void EffectManager::disappear(Element* element)
{

}

void EffectManager::fall(Element* element)
{

}

void EffectManager::pickUp(Element* element)
{

}
