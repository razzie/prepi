#include <ctime>
#include <cmath>
#include "Globals.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "Color.h"
#include "effects\LeafEffect.h"

#define DEFAULT_LEAF_NUM 3

#define MAX(x, y) ((x) > (y) ? (x) : (y))

using namespace irr;

float rand_float(float f)
{
    int rnd = rand() % (int)(f * 256);
    return ((float)rnd / 256.f);
}

video::ITexture* LeafEffect::m_leafTexture = nullptr;

LeafEffect::LeafEffect(Element* element, video::SColor color, core::vector2df direction)
 : m_level(nullptr)
 , m_color(color)
 , m_direction(direction)
 , m_duration((1.f / MAX(direction.getLength(), 0.1f)) * 1000)
 , m_elapsed(0)
{
    if (element != nullptr)
    {
        m_level = element->getLevel();
        m_box = element->getBoundingBox() + element->getPosition();

        if (m_leafTexture == nullptr)
        {
            video::IVideoDriver* driver = m_level->getGlobals()->driver;
            m_leafTexture = driver->getTexture("../media/leaf.png");
        }
    }

    const int leafNum = DEFAULT_LEAF_NUM * m_box.getArea();

    for (int i = 0; i < leafNum; ++i)
    {
        Leaf leaf;
        leaf.m_position.X = m_box.UpperLeftCorner.X + rand_float(m_box.getWidth());
        leaf.m_position.Y = m_box.UpperLeftCorner.Y + rand_float(m_box.getHeight());
        leaf.m_color = m_color;
        randomizeColor(leaf.m_color, 128);
        leaf.m_begin = rand() % 500;
        leaf.m_duration = m_duration + (rand() % 500);
        leaf.m_randomSeed = rand() % 1000;
        m_leafs.push_back(leaf);
    }
}

LeafEffect::~LeafEffect()
{
}

void LeafEffect::update(uint32_t elapsedMs)
{
    if (m_level == nullptr) return;

    for (auto it = m_leafs.begin(), end = m_leafs.end(); it != end; ++it)
    {
        Leaf& leaf = *it;

        // update parameters only when the leaf is visible
        if (m_elapsed >= leaf.m_begin && m_elapsed <= leaf.m_duration)
        {
            // update leaf position
            float speed = (float)elapsedMs / 1000.f;
            core::vector2df movement = m_direction;
            movement.X += m_direction.X + (sin((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);
            movement.Y += m_direction.Y + (cos((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);
            leaf.m_position += movement * speed;

            // update leaf color and alpha
            // randomizeColor(leaf.m_color, 32);
            if (m_elapsed < (leaf.m_begin + 255))
            {
                leaf.m_color.setAlpha(m_elapsed - leaf.m_begin);
            }
            else if (m_elapsed > (leaf.m_duration - 255))
            {
                leaf.m_color.setAlpha(leaf.m_duration - m_elapsed);
            }

            unsigned unit = m_level->getUnitSize();
            video::IVideoDriver* driver = m_level->getGlobals()->driver;
            core::recti screen({0,0}, driver->getScreenSize());

            // setting up leaf display data
            video::SColor leafColor[4] = {leaf.m_color, leaf.m_color, leaf.m_color, leaf.m_color};
            core::vector2di leafPos = m_level->getScreenPosition(leaf.m_position);
            core::recti leafBox(0, 0, unit / 4, unit / 4);
            leafBox += leafPos;

            // draw leaf if it is on screen
            if (screen.isPointInside(leafPos))
            {
                driver->draw2DImage(m_leafTexture, leafBox, core::recti({0, 0}, m_leafTexture->getSize()), 0, leafColor, true);
            }
        }
    }

    m_elapsed += elapsedMs;
}

bool LeafEffect::isFinished() const
{
    return (m_elapsed >= (m_duration + 500));
}
