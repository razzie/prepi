/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <iostream>
#include "Globals.h"
#include "level\Level.h"
#include "effects\TextEffect.h"

using namespace irr;

TextEffect::TextEffect(Level* level, irr::core::stringw text, core::vector2df pos, video::SColor color)
 : m_level(level)
 , m_position(pos)
 , m_elapsed(0)
{
    // splitting the text by new line characters
    m_texts.push_back({{}, color, 0, 1000});
    for (unsigned i = 0, len = text.size(); i < len; ++i)
    {
        if (text[i] == '\n')
        {
            m_texts.push_back({{}, color, 0, 1000});
        }
        else
        {
            Text& t = m_texts.back();
            t.m_text += text[i];
            t.m_duration += 100;
        }
    }

    m_currText = m_texts.begin();
}

TextEffect::~TextEffect()
{
}

void TextEffect::update(uint32_t elapsedMs)
{
    Text& t = *m_currText;

    t.m_posDelta -= elapsedMs / 16;

    if (m_elapsed <= 255)
    {
        t.m_color.setAlpha(m_elapsed);
        t.m_posDelta -= elapsedMs / 8;
    }
    else if (m_elapsed >= (t.m_duration - 255))
    {
        t.m_color.setAlpha(t.m_duration - m_elapsed);
        t.m_posDelta -= elapsedMs / 8;
    }

    core::vector2di pos = m_level->getScreenPosition(m_position);
    pos.Y += t.m_posDelta / 4;

    gui::IGUIFont* font = m_level->getGlobals()->font;
    font->draw(t.m_text, {pos, pos}, t.m_color, true, true);

    m_elapsed += elapsedMs;

    if (m_elapsed > t.m_duration)
    {
        m_elapsed -= t.m_duration;
        ++m_currText;
    }
}

bool TextEffect::isFinished() const
{
    return (m_currText == m_texts.end());
}
