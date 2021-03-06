/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <ctime>
#include "Globals.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "effects\EffectManager.h"
#include "effects\DelayEffect.h"
#include "effects\DamageEffect.h"
#include "effects\TextEffect.h"
#include "effects\ExplosionEffect.h"
#include "effects\MoveElementEffect.h"
#include "effects\AppearEffect.h"
#include "effects\LeafEffect.h"
#include "effects\SmokeEffect.h"

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
        explosion(player->getPosition() + player->getBoundingBox().getCenter(), 0, 0.20f, 0.10f);
    }
}

void EffectManager::text(const char* text, Element* element, video::SColor color)
{
    core::vector2df pos = element->getPosition() + element->getBoundingBox().getCenter() + core::vector2df(0, -element->getScale() / 2.f);
    addEffect( new TextEffect(m_level, text, pos, color) );
}

void EffectManager::text(const wchar_t* text, Element* element, video::SColor color)
{
    core::vector2df pos = element->getPosition() + element->getBoundingBox().getCenter() + core::vector2df(0, -element->getScale() / 2.f);
    addEffect( new TextEffect(m_level, text, pos, color) );
}

void EffectManager::explosion(irr::core::vector2df pos, unsigned image, float scale, float particleSize, irr::video::SColor color)
{
    addEffect( new ExplosionEffect(m_level, pos, image, scale, particleSize, color) );
}

void EffectManager::appear(Element* element)
{
    addEffect( new AppearEffect(element) );
}

void EffectManager::disappear(Element* element)
{
    addEffect( new DisappearEffect(element) );
}

void EffectManager::reappear(Element* element, core::vector2df newPos)
{
    addEffect( new ReappearEffect(element, newPos) );
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
    addEffect( new MoveElementEffect(element, core::vector2di(0, 0), 500) );
}

void EffectManager::leafs(const Shape& shape, unsigned image, video::SColor color, core::vector2df velocity, float length)
{
    addEffect( new LeafEffect(m_level, shape, image, color, velocity, length) );
}

void EffectManager::smoke(Element* element)
{
    addEffect( new SmokeEffect(m_level, element->getPosition() + element->getBoundingBox().getCenter(), element->getScale()) );
}

void EffectManager::smoke(core::vector2df pos, float scale)
{
    addEffect( new SmokeEffect(m_level, pos, scale) );
}
