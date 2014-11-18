#include <ctime>
#include <cmath>
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "effects\DelayEffect.h"
#include "effects\DisappearEffect.h"
#include "elements\Element.h"
#include "behaviors\TeleportBehavior.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

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
 , m_collisionErrorCheck(false)
 , m_collisionErrorTimeout(0)
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
            return;
        }
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

        if (m_sequenceNum > 0) element->enable(false);
    }
}

void TeleportBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    bool playerCollision = m_element->isPlayerCollided();
    if (playerCollision == m_collisionErrorCheck)
    {
        m_collisionErrorTimeout = 0;
    }
    else
    {
        if (m_collisionErrorTimeout > 20)
        {
            m_collisionErrorCheck = playerCollision;
            m_collisionErrorTimeout = 0;
        }
        m_collisionErrorTimeout += elapsedMs;
    }

    //if (m_element->isPlayerCollided())
    if (m_collisionErrorCheck)
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
            //elem->getLevel()->getEffectManager()->disappear(elem);
            Effect* disappearEffect = new DelayEffect(ABS(m_delay), new DisappearEffect(elem));
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
