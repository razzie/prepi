#include <ctime>
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\ParticleElement.h"

using namespace irr;

ParticleElement::ParticleElement(Level* level, unsigned image, core::vector2df position, float scale, video::SColor color, unsigned life)
 : Element(level, Type::PARTICLE, position)
 , m_image(image)
 , m_color(color)
 , m_life(life)
 , m_elapsed(0)
{
    m_id = 1; // 1 for particles, 2 for leafs
    m_scale = scale + ((float)(rand() % 10) / 100.f) - 0.05f;
    m_tileData = level->getTileSet()->getParticleData(1);

    if (m_tileData != nullptr)
    {
        m_imgPosition = m_tileData->getImagePosition(image);
        m_shape = m_tileData->getBoundingShape(m_imgPosition);
        m_shape *= m_scale;
        m_body = m_tileData->createBody(this);
    }
}

ParticleElement::~ParticleElement()
{
}

video::SColor ParticleElement::getColor() const
{
    return m_color;
}

void ParticleElement::setColor(video::SColor color)
{
    m_color = color;
}

void ParticleElement::update(uint32_t elapsedMs)
{
    Element::update(elapsedMs);

    m_elapsed += elapsedMs;
    if (m_elapsed > m_life) this->remove(); // disappears after a while
}

void ParticleElement::draw()
{
    if (m_tileData != nullptr)
    {
        m_tileData->drawAnimation(m_level, m_imgPosition, 0, m_animSpeed, m_position, m_scale, 0.f, m_color);
    }
}
