/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Color.h"
#include "Parser.h"
#include "level\Level.h"
#include "elements\Element.h"
#include "effects\EffectManager.h"
#include "effects\LeafEffect.h"
#include "behaviors\LeafEffectBehavior.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

using namespace irr;

LeafEffectBehavior::LeafEffectBehavior(Element* element, std::istream& stream)
 : LeafEffectBehavior(element, Parser(stream, ',').getArgs<unsigned, video::SColor, core::vector2df, float>())
{
}

LeafEffectBehavior::LeafEffectBehavior(Element* element, std::tuple<unsigned, video::SColor, core::vector2df, float> data)
 : LeafEffectBehavior(element,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data))
{
}

LeafEffectBehavior::LeafEffectBehavior(Element* element, unsigned image, video::SColor color, core::vector2df direction, float length)
 : Behavior(element, Type::LEAF_EFFECT)
 , m_image(image)
 , m_color(color)
 , m_direction(direction)
 , m_length(length)
 , m_genSpeed((1.f / MAX(direction.getLength(), 0.1f)) * 200)
 , m_elapsed(0)
{
}

LeafEffectBehavior::~LeafEffectBehavior()
{
}

void LeafEffectBehavior::update(uint32_t elapsedMs)
{
    if (m_element == nullptr) return;

    m_elapsed += elapsedMs;

    if (m_elapsed >= m_genSpeed)
    {
        m_elapsed = 0;

        Effect* leafEffect = new LeafEffect(m_element, m_image, m_color, m_direction, m_length);
        m_element->getLevel()->getEffectManager()->addEffect(leafEffect);
    }
}
