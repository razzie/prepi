#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

#include <iostream>
#include "irrlicht.h"
#include "tinythread.h"

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
    mutable tthread::mutex m_mutex;
    Level* m_level;
    unsigned m_bgId;
    irr::video::ITexture* m_bg;
    DrawingMethod m_drawingMethod;
};

std::istream& operator>> (std::istream&, Background::DrawingMethod&);

#endif // BACKGROUND_H_INCLUDED
