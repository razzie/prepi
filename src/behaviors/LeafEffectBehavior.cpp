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
 : LeafEffectBehavior(element, Parser(stream, ',').getArgs<video::SColor, core::vector2df>())
{
}

LeafEffectBehavior::LeafEffectBehavior(Element* element, std::tuple<video::SColor, core::vector2df> data)
 : LeafEffectBehavior(element,
    std::get<0>(data),
    std::get<1>(data))
{
}

LeafEffectBehavior::LeafEffectBehavior(Element* element, video::SColor color, core::vector2df direction)
 : Behavior(element, Type::LEAF_EFFECT)
 , m_color(color)
 , m_direction(direction)
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

        Effect* leafEffect = new LeafEffect(m_element, m_color, m_direction);
        m_element->getLevel()->getEffectManager()->addEffect(leafEffect);
    }
}
