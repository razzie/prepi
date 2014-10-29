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

    /*if (m_element->isPlayerCollided())
    {
        m_element->getLevel()->getPlayerElement()->onLadder();
    }*/

    PlayerElement* player = m_element->getLevel()->getPlayerElement();

    if (player == nullptr) return;

    core::vector2df playerPos = player->getPosition() + player->getBoundingBox().getCenter();
    core::rectf ladderBox = m_element->getBoundingBox() + m_element->getPosition();

    if (ladderBox.isPointInside(playerPos))
    //if (m_element->getShape().isPointInside(playerPos - m_element->getPosition()))
    {
        player->onLadder();
    }
}
