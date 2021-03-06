/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <ctime>
#include <cmath>
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "effects\DelayEffect.h"
#include "effects\AppearEffect.h"
#include "elements\Element.h"
#include "behaviors\TeleportBehavior.h"

using namespace irr;

std::map<unsigned, std::vector<TeleportBehavior*>> TeleportBehavior::m_teleports;

TeleportBehavior::TeleportBehavior(Element* element, std::istream& stream)
 : TeleportBehavior(element, Parser(stream, ',').getArgs<unsigned, float, int>())
{
}

TeleportBehavior::TeleportBehavior(Element* element, std::tuple<unsigned, float, int> data)
 : TeleportBehavior(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

TeleportBehavior::TeleportBehavior(Element* element, unsigned sequenceNum, float randomness, int delay)
 : Behavior(element, Type::TELEPORT)
 , m_sequenceNum(sequenceNum)
 , m_randomness(randomness)
 , m_delay(delay)
 , m_touched(false)
 , m_untouchTimer(0)
{
    m_teleports[m_sequenceNum].push_back(this);
}

TeleportBehavior::~TeleportBehavior()
{
    auto& vect = m_teleports[m_sequenceNum];

    for (auto it = vect.begin(), end = vect.end(); it != end; ++it)
    {
        if (*it == this)
        {
            vect.erase(it);
            break;
        }
    }

    if (vect.empty())
    {
        m_teleports.erase(m_sequenceNum);
    }
}

void TeleportBehavior::setElement(Element* element)
{
    Behavior::setElement(element);

    if (element != nullptr)
    {
        // calculating random position
        if (m_randomness > 0.f)
        {
            core::vector2df pos = element->getPosition();
            float angle = (float)(rand() % 1000) / 100.f;
            pos.X += std::sin(angle) * m_randomness;
            pos.Y += std::cos(angle) * m_randomness;

            element->setPosition(pos);
        }

        if (m_sequenceNum > 0) element->setFlags(false);
    }
}

void TeleportBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    bool playerCollision = m_element->isPlayerCollided();
    if (m_touched && !playerCollision)
    {
        if (m_untouchTimer < 50) playerCollision = true;
        m_untouchTimer += elapsedMs;
    }

    if (playerCollision)
    {
        if (m_delay <= 0) activateNext();
        m_touched = true;
    }
    else if (m_touched && m_delay > 0)
    {
        activateNext();
    }
}

void TeleportBehavior::activateNext()
{
    // hide elements in the current sequence
    for (TeleportBehavior* t : m_teleports[m_sequenceNum])
    {
        Element* elem = t->m_element;

        if (elem != nullptr && t->m_delay != 0)
        {
            Effect* disappearEffect = new DelayEffect(std::abs(m_delay), new DisappearEffect(elem));
            elem->getLevel()->getEffectManager()->addEffect(disappearEffect);
        }
    }

    // find next sequence
    auto it = m_teleports.find(m_sequenceNum);
    if (it == m_teleports.end() ||
        std::next(it) == m_teleports.end()) return; // seems like this was the last sequence
    auto& nextSeq = std::next(it)->second;

    // show elements in the next sequence
    for (TeleportBehavior* t : nextSeq)
    {
        Element* elem = t->m_element;

        if (elem != nullptr)
        {
            elem->getLevel()->getEffectManager()->appear(elem);
        }
    }

    delete this;
}
