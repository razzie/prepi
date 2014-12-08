#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include <iostream>
#include "irrlicht.h"
#include "tinythread.h"
#include "Timer.h"

class Level;

class Background
{
public:
    Background(Level*);
    ~Background();

    enum class DrawingMethod : unsigned
    {
        STRETCH = 0,
        VERTICAL = 1,
        HORIZONTAL = 2,
        TILE = 3
    };

    void setId(unsigned);
    void setDrawingMethod(DrawingMethod);
    void draw();

private:
    void resetRandomBg();
    void drawNextRandomBg(irr::video::ITexture*);

    mutable tthread::mutex m_mutex;
    Level* m_level;
    Timer m_timer;
    // normal image background
    unsigned m_bgId;
    irr::video::ITexture* m_bg;
    DrawingMethod m_drawingMethod;
    // random background (if image is not set)
    irr::core::dimension2du m_rndBgDim;
    irr::video::ITexture* m_rndSwapTexture[2];
    unsigned m_rndBgIndex;
    unsigned m_rndBgAdvance;
};

std::istream& operator>> (std::istream&, Background::DrawingMethod&);

#endif // BACKGROUND_H_INCLUDED
