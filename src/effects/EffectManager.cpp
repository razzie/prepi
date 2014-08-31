#include <ctime>
#include "Globals.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "effects\EffectManager.h"
#include "effects\DamageEffect.h"
#include "effects\ExplosionEffect.h"
#include "effects\MoveElementEffect.h"
#include "effects\AppearEffect.h"
#include "effects\DisappearEffect.h"

using namespace irr;

EffectManager::EffectManager(Level* level)
 : m_level(level)
{
}

EffectManager::~EffectManager()
{
    reset();
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

void EffectManager::reset()
{
    for (Effect* effect : m_effects)
    {
        delete effect;
    }

    m_effects.clear();
}

void EffectManager::playerDamage()
{
    // adding red screen to effects
    addEffect( new DamageEffect(m_level) );

    // adding some blood
    Element* player = m_level->getPlayerElement();
    if (player)
    {
        explosion(player->getPosition() + player->getBoundingBox().getCenter(), 0.5f);
    }
}

void EffectManager::explosion(irr::core::vector2df pos, float range, irr::video::SColor color)
{
    addEffect( new ExplosionEffect(m_level, pos, range, color) );
}

void EffectManager::appear(Element* element)
{
    addEffect( new AppearEffect(element) );
}

void EffectManager::disappear(Element* element)
{
    addEffect( new DisappearEffect(element) );
}

void EffectManager::fall(Element* element)
{
    core::dimension2du screenSize = m_level->getGlobals()->driver->getScreenSize();
    core::vector2di endPos = m_level->getScreenPosition(element);
    endPos.Y = screenSize.Height;
    addEffect( new MoveElementEffect(element, endPos) );
}

void EffectManager::pickUp(Element* element)
{
    addEffect( new MoveElementEffect(element, core::vector2di(0, 0)) );
}
