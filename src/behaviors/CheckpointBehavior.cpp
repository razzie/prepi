/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "level\Level.h"
#include "elements\Element.h"
#include "elements\PlayerElement.h"
#include "effects\EffectManager.h"
#include "behaviors\CheckpointBehavior.h"

using namespace irr;

CheckpointBehavior::CheckpointBehavior(Element* element)
 : Behavior(element, Type::CHECKPOINT)
{
}

CheckpointBehavior::~CheckpointBehavior()
{
}

void CheckpointBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    if (m_element->isPlayerCollided())
    {
        Level* level = m_element->getLevel();
        PlayerElement* player = level->getPlayerElement();

        player->checkpoint();
        level->getEffectManager()->text("CHECKPOINT", m_element, {255, 100, 255, 100});
        delete this;
        return;
    }
}
