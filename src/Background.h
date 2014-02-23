#ifndef BACKGROUND_H_INCLUDED
#define BACKGROUND_H_INCLUDED

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
    void update();

private:
    mutable tthread::mutex m_mutex;
    Level* m_level;
    irr::video::ITexture* m_bg;
    DrawingMethod m_drawingMethod;
};

#endif // BACKGROUND_H_INCLUDED
