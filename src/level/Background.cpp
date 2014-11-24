#include <map>
#include "simplex.h"
#include "Globals.h"
#include "TileSet.h"
#include "Level.h"
#include "Background.h"

using namespace irr;

std::istream& operator>> (std::istream& i, Background::DrawingMethod& dm)
{
    unsigned drawingMethod;
    i >> drawingMethod;
    dm = static_cast<Background::DrawingMethod>(drawingMethod);
    return i;
}


Background::Background(Level* level)
 : m_level(level)
 , m_bgId(0)
 , m_bg(nullptr)
 , m_drawingMethod(DrawingMethod::STRETCH)
 , m_randomBgs{nullptr, nullptr}
 , m_randomBg(0)
 , m_randomBgCounter(0)
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

void Background::draw()
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);

    video::IVideoDriver* driver = m_level->getGlobals()->driver;
    const unsigned unit = m_level->getUnitSize();
    const irr::core::rect<irr::s32> view = m_level->getView();
    const core::dimension2du levelDim = m_level->getDimension();
    core::rect<s32> levelRect = {0, 0, (s32)(levelDim.Width * unit), (s32)(levelDim.Height * unit)};

    if (m_bgId && m_bg == nullptr)
    {
        m_bg = m_level->getTileSet()->getBackground(m_bgId);
    }

    if (m_bg)
    {
        const core::dimension2du bgDim = m_bg->getOriginalSize();
        const core::rect<s32> srcRect = {{0, 0}, bgDim};
        core::rect<s32> destRect = {0, 0, 0, 0};
        const double ratio = (double)bgDim.Width / (double)bgDim.Height;
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

        destRect -= view.UpperLeftCorner;
        levelRect -= view.UpperLeftCorner;

        auto clipRect = levelRect;
        clipRect.clipAgainst( {{0, 0}, view.getSize()} );

        unsigned xr, yr;
        for (xr = 0; xr < xRepeats; ++xr) // column repeats
        {
            for (yr = 0; yr < yRepeats; ++yr) // row repeats
            {
                driver->draw2DImage(m_bg, destRect, srcRect, &clipRect);
                destRect += {0, destRect.getHeight()};
            }
            destRect += {destRect.getWidth(),
                (s32)(-yr * destRect.getHeight())}; // restore previous Y position
        }
    }
    else
    {
        const core::dimension2du bgDim = levelDim * 2;

        if (m_randomBgs[0] == nullptr)
            m_randomBgs[0] = driver->addRenderTargetTexture(bgDim, "rbg1");
        if (m_randomBgs[1] == nullptr)
            m_randomBgs[1] = driver->addRenderTargetTexture(bgDim, "rbg2");

        if (m_timer.peekElapsed() >= 512)
        {
            video::ITexture* bg = m_randomBgs[m_randomBg];
            driver->setRenderTarget(bg); // draw to this texture

            for (unsigned w = 0; w < bgDim.Width; ++w)
            {
                for (unsigned h = 0; h < bgDim.Height; ++h)
                {
                    const float rate = 0.125f;
                    float n = noise(rate * w, rate * h, rate * m_randomBgCounter);
                    video::SColor color(255, n * 64, 0, 0);
                    driver->drawPixel(w, h, color);
                }
            }

            driver->setRenderTarget(video::ERT_FRAME_BUFFER); // restore render target
            m_timer.reset();
            m_randomBg = 1 - m_randomBg;
            ++m_randomBgCounter;
        }

        const core::rect<s32> srcRect({0, 0}, bgDim);
        const core::rect<s32> destRect(-view.UpperLeftCorner, -view.UpperLeftCorner + core::vector2di(levelDim.Width * unit, levelDim.Height * unit));
        const unsigned elapsedMs = m_timer.peekElapsed();
        const uint8_t alpha = (elapsedMs >= 512 ? 255 : elapsedMs / 2);
        const video::SColor alphaColors[4] = {{alpha, 255, 255, 255},
                                              {alpha, 255, 255, 255},
                                              {alpha, 255, 255, 255},
                                              {alpha, 255, 255, 255}};

        if (alpha < 255) driver->draw2DImage(m_randomBgs[m_randomBg], destRect, srcRect);
        driver->draw2DImage(m_randomBgs[1 - m_randomBg], destRect, srcRect, 0, alphaColors);
    }
}
