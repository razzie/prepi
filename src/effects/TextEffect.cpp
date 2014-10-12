#include "Globals.h"
#include "level\Level.h"
#include "effects\TextEffect.h"

#define DURATION 1000

using namespace irr;

gui::IGUIFont* TextEffect::m_font = nullptr;

TextEffect::TextEffect(Level* level, irr::core::stringw text, core::vector2df pos, video::SColor color)
 : m_level(level)
 , m_text(text)
 , m_position(pos)
 , m_color(color)
 , m_duration(0)
 , m_elapsed(0)
{
    // load font bitmap if not loaded yet
    if (m_font == nullptr)
    {
        m_font = m_level->getGlobals()->guienv->getFont("../media/font.png");
    }

    // calculating text dimension (including line breaks) + duration
    core::dimension2du kerning(m_font->getKerningWidth(), m_font->getKerningHeight());
    core::dimension2du dim(0, kerning.Height);

    for (unsigned i = 0, len = m_text.size(); i < len; ++i)
    {
        if (m_text[i] == '\n')
            dim.Height += kerning.Height;
        else
            dim.Width += kerning.Width;
    }

    m_textBox.UpperLeftCorner.set(-dim.Width / 2, -dim.Height);
    m_textBox.LowerRightCorner.set(dim.Width / 2, 0);

    m_duration = m_text.size() * 200;
}

TextEffect::~TextEffect()
{
}

void TextEffect::update(uint32_t elapsedMs)
{
    // update alpha channel
    if (m_elapsed <= 255)
    {
        m_color.setAlpha(m_elapsed);
    }
    else if (m_elapsed >= (m_duration - 255))
    {
        m_color.setAlpha(m_duration - m_elapsed);
    }

    // calculate data for drawing
    core::vector2di pos = m_level->getScreenPosition(m_position);
    pos.Y -= m_elapsed / 10;
    //core::recti box(pos.X - 100, pos.Y - 20, pos.X + 100, pos.Y + 20);

    // draw
    m_font->draw(m_text, m_textBox + pos, m_color, true, false);

    m_elapsed += elapsedMs;
}

bool TextEffect::isFinished() const
{
    return (m_elapsed >= m_duration);
}
