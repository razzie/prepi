#ifndef TEXTEFFECT_H_INCLUDED
#define TEXTEFFECT_H_INCLUDED

#include <vector>
#include "irrlicht.h"
#include "effects\Effect.h"

class Level;

class TextEffect : public Effect
{
public:
    TextEffect(Level*, irr::core::stringw, irr::core::vector2df, irr::video::SColor = {255, 255, 255, 255});
    ~TextEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    struct Text
    {
        irr::core::stringw m_text;
        irr::video::SColor m_color;
        int m_posDelta;
        uint32_t m_duration;
    };

    Level* m_level;
    std::vector<Text> m_texts;
    std::vector<Text>::iterator m_currText;
    irr::core::vector2df m_position;
    uint32_t m_elapsed;
};

#endif // TEXTEFFECT_H_INCLUDED
