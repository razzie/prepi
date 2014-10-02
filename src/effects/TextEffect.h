#ifndef TEXTEFFECT_H_INCLUDED
#define TEXTEFFECT_H_INCLUDED

#include <string>
#include "irrlicht.h"
#include "effects\Effect.h"

class Level;

class TextEffect : public Effect
{
public:
    TextEffect(Level*, const wchar_t*, irr::core::vector2df, irr::video::SColor = {255, 255, 255, 255});
    ~TextEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    static irr::gui::IGUIFont* m_font;
    Level* m_level;
    std::wstring m_text;
    irr::core::vector2df m_position;
    irr::video::SColor m_color;
    uint32_t m_elapsed;
};

#endif // TEXTEFFECT_H_INCLUDED
