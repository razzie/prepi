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

    Level* level = m_element->getLevel();
    PlayerElement* player = level->getPlayerElement();

    if (player == nullptr) return;

    core::rectf playerBox = player->getBoundingBox() + player->getPosition();
    core::rectf ladderBox = m_element->getBoundingBox() + m_element->getPosition();

    if (ladderBox.isRectCollided(playerBox))
    {
        player->onLadder();
    }
}
