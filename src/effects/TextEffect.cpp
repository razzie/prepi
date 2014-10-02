#include "Globals.h"
#include "level\Level.h"
#include "effects\TextEffect.h"

#define DURATION 1000

using namespace irr;

gui::IGUIFont* TextEffect::m_font = nullptr;

TextEffect::TextEffect(Level* level, const wchar_t* text, core::vector2df pos, video::SColor color)
 : m_level(level)
 , m_text(text)
 , m_position(pos)
 , m_color(color)
 , m_elapsed(0)
{
    if (m_font == nullptr)
    {
        m_font = m_level->getGlobals()->guienv->getFont("../media/font.png");
    }
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
    else if (m_elapsed >= (DURATION - 255))
    {
        m_color.setAlpha(DURATION - m_elapsed);
    }

    // calculate data for drawing
    core::vector2di pos = m_level->getScreenPosition(m_position);
    pos.Y -= m_elapsed / 10;
    core::recti box(pos.X - 100, pos.Y - 20, pos.X + 100, pos.Y + 20);

    // draw
    m_font->draw(m_text.c_str(), box, m_color, true, true);

    m_elapsed += elapsedMs;
}

bool TextEffect::isFinished() const
{
    return (m_elapsed >= DURATION);
}
