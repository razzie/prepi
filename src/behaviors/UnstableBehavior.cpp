#include "Box2D\Box2D.h"
#include "Parser.h"
#include "elements\Element.h"
#include "behaviors\UnstableBehavior.h"

UnstableBehavior::UnstableBehavior(Element* element, std::istream& stream)
 : UnstableBehavior(element, Parser(stream, ',').getArg<bool>())
{
}

UnstableBehavior::UnstableBehavior(Element* element, bool disappear)
 : Behavior(element, Type::UNSTABLE)
 , m_triggered(false)
 , m_disappear(disappear)
{
}

UnstableBehavior::~UnstableBehavior()
{
}

void UnstableBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr || m_triggered) return; // no element or already triggered (fallen down)

    m_element->updateCollisions();
    auto collisions = m_element->getCollisions();

    for (auto collision : collisions)
    {
        Element* contactElem = collision.getOtherElement();

        if (contactElem->getType() == Element::Type::PLAYER)
        {
            if (m_disappear)
            {
                m_element->remove();
            }
            else
            {
                m_element->getBody()->SetType(b2_dynamicBody);
            }

            m_triggered = true;
            return;
        }
    }
}
