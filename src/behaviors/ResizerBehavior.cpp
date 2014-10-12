#include "Parser.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "level\Level.h"
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

    if (m_element->isPlayerCollided())
    {
        m_element->getLevel()->getPlayerElement()->setScale(m_scale);
        m_element->remove();
    }
}
