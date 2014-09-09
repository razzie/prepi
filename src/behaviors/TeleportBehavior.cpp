#include <ctime>
#include <cmath>
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "elements\Element.h"
#include "behaviors\TeleportBehavior.h"

using namespace irr;

std::map<unsigned, std::vector<TeleportBehavior*>> TeleportBehavior::m_teleports;

TeleportBehavior::TeleportBehavior(Element* element, std::istream& stream)
 : TeleportBehavior(element, Parser(stream, ',').getArgs<unsigned, float>())
{
}

TeleportBehavior::TeleportBehavior(Element* element, std::tuple<unsigned, float> data)
 : TeleportBehavior(element,
    std::get<0>(data),
    std::get<1>(data))
{
}

TeleportBehavior::TeleportBehavior(Element* element, unsigned sequenceNum, float randomness)
 : Behavior(element, Type::TELEPORT)
 , m_sequenceNum(sequenceNum)
 , m_randomness(randomness)
 , m_triggered(false)
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

    if (element && m_sequenceNum > 0)
    {
        element->enable(false);
    }
}

void TeleportBehavior::update(uint32_t)
{
    if (m_element == nullptr) return;

    if (!m_triggered)
    {
        m_element->updateCollisions();
        auto collisions = m_element->getCollisions();

        for (auto collision : collisions)
        {
            Element* contactElem = collision.getOtherElement();

            if (contactElem->getType() == Element::Type::PLAYER)
            {
                activateNext();
                m_triggered = true;
                return;
            }
        }
    }
}

void TeleportBehavior::activateNext()
{
    // hide elements in the current sequence
    for (TeleportBehavior* t : m_teleports[m_sequenceNum])
    {
        Element* elem = t->m_element;

        if (elem)
        {
            //elem->enable(false);
            elem->getLevel()->getEffectManager()->disappear(elem);
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

        if (elem)
        {
            // calculating random position
            if (m_randomness > 0.f)
            {
                core::vector2df pos = elem->getPosition();
                float angle = (float)(rand() % 1000) / 100.f;
                pos.X += std::sin(angle) * m_randomness;
                pos.Y += std::cos(angle) * m_randomness;

                elem->setPosition(pos);
            }

            //elem->enable(true);
            elem->getLevel()->getEffectManager()->appear(elem);
        }
    }
}
