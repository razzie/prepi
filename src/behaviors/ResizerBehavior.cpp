#include "Parser.h"
#include "elements\Element.h"
#include "behaviors\ResizerBehavior.h"

ResizerBehavior::ResizerBehavior(Element* element, std::istream& stream)
 : ResizerBehavior(element, Parser(stream, ',').getArg<float>())
{
}

ResizerBehavior::ResizerBehavior(Element* element, float scale)
 : Behavior(element, Type::RESIZER)
 , m_scale(scale)
{
}

ResizerBehavior::~ResizerBehavior()
{
}

void ResizerBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    m_element->updateCollisions();
    auto collisions = m_element->getCollisions();

    for (auto collision : collisions)
    {
        Element* contactElem = collision.getOtherElement();

        if (contactElem->getType() == Element::Type::PLAYER)
        {
            contactElem->setScale(m_scale);
            m_element->remove();
            return;
        }
    }
}
