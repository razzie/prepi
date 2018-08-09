/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <ctime>
#include <cmath>
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\Element.h"
#include "Color.h"
#include "effects\LeafEffect.h"

#define DEFAULT_LEAF_NUM 4.5f

#define MAX(x, y) ((x) > (y) ? (x) : (y))

using namespace irr;

static float randFloat(float f)
{
    int rnd = rand() % (int)(f * 256);
    return ((float)rnd / 256.f);
}

static core::vector2df randPointInShape(const Shape& shape)
{
    core::vector2df p;
    core::rectf box = shape.getBoxData();

    do
    {
        p.X = box.UpperLeftCorner.X + randFloat(box.getWidth() - 0.18f) + 0.09f;
        p.Y = box.UpperLeftCorner.Y + randFloat(box.getHeight() - 0.18f) + 0.09f;
    }
    while ( !shape.isPointInside(p) );

    return p;
}

LeafEffect::LeafEffect(Element* element, unsigned image, video::SColor color, core::vector2df velocity, float length)
 : LeafEffect(element->getLevel(), element->getShape() + element->getPosition(), image, color, velocity, length)
{
}

LeafEffect::LeafEffect(Level* level, const Shape& shape, unsigned image, video::SColor color, core::vector2df velocity, float length)
 : m_level(level)
 , m_tileData(nullptr)
 , m_imgPos(0, 0)
 , m_color(color)
 , m_velocity(velocity)
 , m_shape(shape)
 , m_duration((1.f / MAX(velocity.getLength(), 0.1f)) * 500.f * length)
 , m_elapsed(0)
{
    m_tileData = m_level->getTileSet()->getParticleData(2);
    if (m_tileData != nullptr)
    {
        m_imgPos = m_tileData->getImagePosition(image);
    }

    const int leafNum = MAX(1, (float)DEFAULT_LEAF_NUM * std::sqrt(m_shape.getArea()));

    for (int i = 0; i < leafNum; ++i)
    {
        Leaf leaf;
        leaf.m_position = randPointInShape(m_shape);
        leaf.m_color = m_color;
        randomizeColor(leaf.m_color, 64);
        leaf.m_begin = rand() % 500;
        leaf.m_duration = m_duration + (rand() % 1000) - 500;
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

    for (auto& leaf : m_leafs)
    {
        // update parameters only when the leaf is visible
        if (m_elapsed >= leaf.m_begin && m_elapsed <= leaf.m_duration)
        {
            // calculate leaf movement
            float speed = (float)elapsedMs / 1000.f;
            core::vector2df movement = m_velocity;
            movement.X += m_velocity.X + (sin((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);
            movement.Y += m_velocity.Y + (cos((leaf.m_randomSeed + m_elapsed) / 256) * 0.5f);

            // update leaf alpha and position
            if (m_elapsed < (leaf.m_begin + 255))
            {
                leaf.m_color.setAlpha(m_elapsed - leaf.m_begin);
                leaf.m_position += movement * speed * ((float)(m_elapsed - leaf.m_begin) / 255.f);
            }
            else if (m_elapsed > (leaf.m_duration - 255))
            {
                leaf.m_color.setAlpha(leaf.m_duration - m_elapsed);
                leaf.m_position += movement * speed;
            }
            else
            {
                leaf.m_position += movement * speed;
            }

            // drawing leaf
            m_tileData->drawAnimation(m_level, m_imgPos, 0, m_level->getSpeed(), leaf.m_position, 0.25f, 0.f, leaf.m_color, leaf.m_randomSeed);
        }
    }

    m_elapsed += elapsedMs;
}

bool LeafEffect::isFinished() const
{
    return (m_elapsed >= (m_duration + 500));
}
