#include "Globals.h"
#include "level\Level.h"
#include "effects\DamageEffect.h"

#define DURATION 1000

using namespace irr;

video::ITexture* DamageEffect::m_texture = nullptr;

DamageEffect::DamageEffect(Level* level)
 : m_level(level)
 , m_elapsed(0)
{
    if (m_texture == nullptr)
    {
        video::IVideoDriver* driver = m_level->getGlobals()->driver;
        m_texture = driver->getTexture("../media/damage.png");
    }
}

DamageEffect::~DamageEffect()
{
}

void DamageEffect::update(uint32_t elapsedMs)
{
    video::IVideoDriver* driver = m_level->getGlobals()->driver;
    core::recti screen({0,0}, driver->getScreenSize());

    float step = ((float)DURATION / 100.f);
    float alpha = (m_elapsed > DURATION) ? 0.f : ((float)(DURATION - m_elapsed) / step);
    video::SColor color[4] = {
        video::SColor((uint8_t)alpha, 255, 255, 255),
        video::SColor((uint8_t)alpha, 255, 255, 255),
        video::SColor((uint8_t)alpha, 255, 255, 255),
        video::SColor((uint8_t)alpha, 255, 255, 255) };

    //driver->draw2DRectangle(video::SColor((uint8_t)alpha, 255, 0, 0), screen);
    driver->draw2DImage(m_texture, screen, core::recti({0, 0}, m_texture->getSize()), 0, color, true);

    m_elapsed += elapsedMs;
}

bool DamageEffect::isFinished() const
{
    return (m_elapsed > DURATION);
}
