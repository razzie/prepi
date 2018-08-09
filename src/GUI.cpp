/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Globals.h"
#include "level\Level.h"
#include "elements\PlayerElement.h"
#include "GUI.h"

using namespace irr;

GUI::GUI(Globals* globals, Level* level)
 : m_globals(globals)
 , m_level(level)
{
}

GUI::~GUI()
{
}

void GUI::setLevel(Level* level)
{
    m_level = level;
}

void GUI::update()
{
    if (m_level)
    {
        PlayerElement* player = m_level->getPlayerElement();
        if (player)
        {
            drawHealthBar();
            drawRewardBar();
        }
    }
}

float GUI::getHealthRate()
{
    if (m_level == nullptr) return 0.f;

    PlayerElement* player = m_level->getPlayerElement();
    if (player == nullptr) return 0.f;

    return ((float)player->getHealth() / player->getMaxHealth());
}

float GUI::getRewardRate()
{
    if (m_level == nullptr) return 0.f;

    PlayerElement* player = m_level->getPlayerElement();
    if (player == nullptr) return 0.f;

    return ((float)player->getRewards() / m_level->getRewardSum());
}

void GUI::drawHealthBar()
{
    m_globals->driver->draw2DRectangle({255,255,0,0}, {5, 5, (s32)(getHealthRate() * 200), 15});
}

void GUI::drawRewardBar()
{
    m_globals->driver->draw2DRectangle({255,255,255,0}, {5, 20, (s32)(getRewardRate() * 200), 30});
}
