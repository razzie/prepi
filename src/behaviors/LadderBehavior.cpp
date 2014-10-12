#include "irrlicht.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "behaviors\LadderBehavior.h"

using namespace irr;

extern float gravity;

LadderBehavior::LadderBehavior(Element* element)
 : Behavior(element, Type::LADDER)
{
}

LadderBehavior::~LadderBehavior()
{
}

void LadderBehavior::update(uint32_t)
{
    if (m_element == nullptr) return;

    if (m_element->isPlayerCollided())
    {
        m_element->getLevel()->getPlayerElement()->onLadder();
    }
}
