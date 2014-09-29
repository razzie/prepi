#include <ctime>
#include <cmath>
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\Element.h"
#include "Color.h"
#include "effects\LeafEffect.h"

#define DEFAULT_LEAF_NUM 1.5f

#define MAX(x, y) ((x) > (y) ? (x) : (y))

using namespace irr;

float rand_float(float f)
{
    int rnd = rand() % (int)(f * 256);
    return ((float)rnd / 256.f);
}

LeafEffect::LeafEffect(Element* element, unsigned image, video::SColor color, core::vector2df velocity, float length)
 : m_level(nullptr)
 , m_tileData(nullptr)
 , m_imgPos(0, 0)
 , m_color(color)
 , m_velocity(velocity)
 , m_duration((1.f / MAX(velocity.getLength(), 0.1f)) * 500.f * length)
 , m_elapsed(0)
{
    if (element != nullptr)
    {
        m_level = element->getLevel();

        m_tileData = m_level->getTileSet()->getParticleData(1);
        if (m_tileData != nullptr)
        {
            m_imgPos = m_tileData->getImagePosition(image);
        }

        m_box = element->getBoundingBox() + element->getPosition();
    }

    const int leafNum = MAX(1, (float)DEFAULT_LEAF_NUM * std::sqrt(m_box.getArea()));

    for (int i = 0; i < leafNum; ++i)
    {
        Leaf leaf;
        leaf.m_position.X = m_box.UpperLeftCorner.X + rand_float(m_box.getWidth() - 0.18f) + 0.08f;
        leaf.m_position.Y = m_box.UpperLeftCorner.Y + rand_float(m_box.getHeight() - 0.18f) + 0.08f;
        leaf.m_color = m_color;
        randomizeColor(leaf.m_color, 64);
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
            core::vector2df movement = m_velocity;
            movement.X += m_velocity.X + (sin((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);
            movement.Y += m_velocity.Y + (cos((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);
            leaf.m_position += movement * speed;

            // update leaf color and alpha
            if (m_elapsed < (leaf.m_begin + 255))
            {
                leaf.m_color.setAlpha(m_elapsed - leaf.m_begin);
            }
            else if (m_elapsed > (leaf.m_duration - 255))
            {
                leaf.m_color.setAlpha(leaf.m_duration - m_elapsed);
            }

            // drawing leaf
            m_tileData->drawAnimation(0, 5, m_level, m_imgPos, leaf.m_position, 0.25f, false, leaf.m_color);
        }
    }

    m_elapsed += elapsedMs;
}

bool LeafEffect::isFinished() const
{
    return (m_elapsed >= (m_duration + 500));
}
