/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Box2D\Box2D.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "behaviors\TrapBehavior.h"

using namespace irr;

TrapBehavior::TrapBehavior(Element* element)
 : Behavior(element, Type::TRAP)
{
}

TrapBehavior::~TrapBehavior()
{
}

void TrapBehavior::update(uint32_t elapsedMs)
{
    class HitTest : public b2QueryCallback
    {
        bool m_otherElementBlocks;

    public:
        HitTest() : m_otherElementBlocks(false) {}

        bool ReportFixture(b2Fixture* fixture)
        {
            Element* elem = static_cast<Element*>(fixture->GetBody()->GetUserData());
            Element::Type elemType = elem->getType();

            if (//elem != m_element &&
                elemType != Element::Type::PARTICLE &&
                elemType != Element::Type::PLAYER)
            {
                m_otherElementBlocks = true;
                return false;
            }

            return true;
        }

        bool otherElementBlocks() const
        {
            return m_otherElementBlocks;
        }
    };

    if (m_element == nullptr) return;

    Level* level = m_element->getLevel();
    PlayerElement* player = level->getPlayerElement();

    if (player == nullptr) return;

    core::rectf trapBox = m_element->getBoundingBox() + m_element->getPosition();
    core::rectf playerBox = player->getBoundingBox() +  player->getPosition();

    if (playerBox.UpperLeftCorner.Y > trapBox.LowerRightCorner.Y &&
        playerBox.LowerRightCorner.X > trapBox.UpperLeftCorner.X &&
        playerBox.UpperLeftCorner.X < trapBox.LowerRightCorner.X)
    {
        HitTest hitTest;
        b2AABB aabb;
        aabb.lowerBound.Set(trapBox.UpperLeftCorner.X + 0.2f, trapBox.LowerRightCorner.Y + 0.2f);
        aabb.upperBound.Set(trapBox.LowerRightCorner.X - 0.2f, playerBox.UpperLeftCorner.Y - 0.2f);

        level->getPhysics()->QueryAABB(&hitTest, aabb);

        if (!hitTest.otherElementBlocks() &&
            m_element->getBody() != nullptr)
        {
            m_element->getBody()->SetType(b2_dynamicBody);
            delete this;
            return;
        }
    }
}
