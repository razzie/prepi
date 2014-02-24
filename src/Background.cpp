#include <map>
#include "Background.h"
#include "Level.h"
#include "Globals.h"

using namespace irr;

static std::map<unsigned, io::path> bgs = {
    {1, "../media/tilesets/background/01_Fantasy.jpg"},
    {2, "../media/tilesets/background/02_Magic.jpg"},
    {3, "../media/tilesets/background/03_Clear_sky.png"},
    {4, "../media/tilesets/background/04_Dark.png"}};

std::istream& operator>> (std::istream& i, Background::DrawingMethod& dm)
{
    unsigned drawingMethod;
    i >> drawingMethod;
    dm = static_cast<Background::DrawingMethod>(drawingMethod);
    return i;
}


Background::Background(Level* level)
 : m_level(level)
 , m_bg(nullptr)
 , m_drawingMethod(DrawingMethod::STRETCH)
{
}

Background::~Background()
{
}

void Background::setId(unsigned id)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_bgId = id;
    m_bg = nullptr;
}

void Background::setDrawingMethod(DrawingMethod dm)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_drawingMethod = dm;
}

void Background::update()
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);

    if (m_bgId && m_bg == nullptr)
    {
        m_bg = m_level->getGlobals()->driver->getTexture(bgs[m_bgId]);
    }

    if (m_bg)
    {
        Globals* globals = m_level->getGlobals();
        core::dimension2du levelDim = m_level->getDimension();
        core::dimension2du bgDim = m_bg->getOriginalSize();
        unsigned unit = m_level->getUnitSize();
        core::rect<s32> srcRect = {{0,0}, bgDim};
        core::rect<s32> destRect = {0,0,0,0};
        core::rect<s32> levelRect = {0, 0, (s32)(levelDim.Width*unit), (s32)(levelDim.Height*unit)};
        double ratio = (double)bgDim.Width / (double)bgDim.Height;
        unsigned xRepeats = 1, yRepeats = 1;

        switch (m_drawingMethod)
        {
        case DrawingMethod::STRETCH:
            destRect = levelRect;
            break;

        case DrawingMethod::VERTICAL:
            destRect = {0, 0, levelRect.getWidth(), (s32)(levelRect.getWidth() / ratio)};
            yRepeats = levelRect.getHeight() / destRect.getHeight() + 1;
            break;

        case DrawingMethod::HORIZONTAL:
            destRect = {0, 0, (s32)(levelRect.getHeight() * ratio), levelRect.getHeight()};
            xRepeats = levelRect.getWidth() / destRect.getWidth() + 1;
            break;

        case DrawingMethod::TILE:
            destRect = srcRect;
            xRepeats = levelRect.getWidth() / bgDim.Width + 1;
            yRepeats = levelRect.getHeight() / bgDim.Height + 1;
            break;

        default:
            destRect = srcRect;
            break;
        }

        irr::core::rect<irr::s32> view = m_level->getView();
        core::position2d<s32> viewOffset = {view.UpperLeftCorner.X % destRect.getWidth(), view.UpperLeftCorner.Y % destRect.getHeight()};
        destRect -= viewOffset; // moving the background starting position according to the view
        levelRect -= viewOffset;

        auto clipRect = levelRect;
        clipRect.clipAgainst(view);

        unsigned xr, yr;
        for (xr = 0; xr < xRepeats; ++xr) // column repeats
        {
            for (yr = 0; yr < yRepeats; ++yr) // row repeats
            {
                globals->driver->draw2DImage(m_bg, destRect, srcRect, &clipRect);
                destRect += {0, destRect.getHeight()};
            }
            destRect += {destRect.getWidth(),
                (s32)(-yr * destRect.getHeight())}; // restore previous Y position
        }
        //globals->driver->draw2DRectangleOutline(levelRect);
    }
}
