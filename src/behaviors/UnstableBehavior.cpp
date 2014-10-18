#include "Box2D\Box2D.h"
#include "Parser.h"
#include "elements\Element.h"
#include "behaviors\UnstableBehavior.h"

UnstableBehavior::UnstableBehavior(Element* element, std::istream& stream)
 : UnstableBehavior(element, Parser(stream, ',').getArg<int>()) // <bool> doesn't work
{
}

UnstableBehavior::UnstableBehavior(Element* element, bool disappear)
 : Behavior(element, Type::UNSTABLE)
 , m_disappear(disappear)
{
}

UnstableBehavior::~UnstableBehavior()
{
}

void UnstableBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    if (m_element->isPlayerCollided())
    {
        if (m_disappear)
        {
            m_element->remove();
        }
        else if (m_element->getBody() != nullptr)
        {
            m_element->getBody()->SetType(b2_dynamicBody);
        }

        delete this;
    }
}
