/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <ctime>
#include "irrlicht.h"
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "elements\ParticleElement.h"
#include "behaviors\ExplosiveBehavior.h"

using namespace irr;

ExplosiveBehavior::ExplosiveBehavior(Element* element, std::istream& stream)
 : ExplosiveBehavior(element, Parser(stream, ',').getArgs<unsigned, unsigned, float>())
{
}

ExplosiveBehavior::ExplosiveBehavior(Element* element, std::tuple<unsigned, unsigned, float> data)
 : ExplosiveBehavior(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

ExplosiveBehavior::ExplosiveBehavior(Element* element, unsigned damage, unsigned timeout, float range)
 : Behavior(element, Type::UNSTABLE)
 , m_triggered(false)
 , m_explosionTimer(0)
 , m_damage(damage)
 , m_timeout(timeout)
 , m_range((range == 0) ? 1 : range)
{
}

ExplosiveBehavior::~ExplosiveBehavior()
{
}

void ExplosiveBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return; // no element or already triggered (fallen down)

    if (m_triggered)
    {
        m_explosionTimer += elapsedMs;

        if (m_explosionTimer >= m_timeout)
        {
            // damaging player
            PlayerElement* player = m_element->getLevel()->getPlayerElement();
            if (player &&
                m_element->getPosition().getDistanceFrom(player->getPosition()) <= m_range)
            {
                player->takeDamage(m_damage);
            }

            // explosion effect
            core::vector2df explosionPos = m_element->getPosition() + m_element->getBoundingBox().getCenter();
            m_element->getLevel()->getEffectManager()->explosion(explosionPos, 0, m_range, 0.16f, video::SColor(0, 255, 100, 0));
            m_element->getLevel()->getEffectManager()->smoke(explosionPos, m_range * 0.8f);

            // remove exploded element from scene
            m_element->remove();
        }
    }
    else
    {
        if (m_element->isPlayerCollided())
        {
            m_triggered = true;
        }
    }
}
