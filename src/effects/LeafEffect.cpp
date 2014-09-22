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
        leaf.m_begin = rand() % 500;
        leaf.m_duration = m_duration + (rand() % 500);
        leaf.m_randomSeed = rand() % 500;
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
        // update leaf position
        float speed = (float)elapsedMs / 1000.f;
        core::vector2df movement = m_direction;
        movement.X += m_direction.X + sin(it->m_randomSeed + m_elapsed);
        movement.Y += m_direction.Y + cos(it->m_randomSeed + m_elapsed);
        it->m_position += movement * speed;

        // update leaf color
        randomizeColor(it->m_color, 16);

        // draw leaf
        if (m_elapsed >= it->m_begin && m_elapsed <= it->m_duration)
        {
            unsigned unit = m_level->getUnitSize();
            video::IVideoDriver* driver = m_level->getGlobals()->driver;
            core::recti screen({0,0}, driver->getScreenSize());

            video::SColor leafColor[4] = {it->m_color, it->m_color, it->m_color, it->m_color};
            core::vector2di leafPos = m_level->getScreenPosition(it->m_position);
            core::recti leafBox(0, 0, unit / 4, unit / 4);
            leafBox += leafPos;

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
