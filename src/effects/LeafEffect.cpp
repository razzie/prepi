#include "effects\LeafEffect.h"

#define DURATION 2000

using namespace irr;

LeafEffect::LeafEffect(Element* element, video::SColor color, core::vector2df direction)
 : ElementEffect(element)
 , m_color(color)
 , m_direction(direction)
 , m_elapsed(0)
{
}

LeafEffect::~LeafEffect()
{
}

void LeafEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed == 0) // first call
    {
        // todo
    }

    m_elapsed += elapsedMs;
}

bool LeafEffect::isFinished() const
{
    return (m_elapsed >= DURATION);
}
