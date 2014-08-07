#include <ctime>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "level\Level.h"
#include "elements\ParticleElement.h"

using namespace irr;

ParticleElement::ParticleElement(Level* level, video::SColor color, core::vector2df position, unsigned life)
 : Element(level, Type::PARTICLE, position)
 , m_color(color)
 , m_life(life)
 , m_size(0.03f + ((float)(rand() % 40) / 1000.f))
 , m_elapsed(0)
{
    m_boundingBox = core::rectf(-m_size, -m_size, m_size, m_size);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(m_position.X, m_position.Y);
    bodyDef.fixedRotation = true; // do not rotate!
    bodyDef.userData = this;
    m_body = m_level->getPhysics()->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_p.Set(-1.f, -1.f);
    circleShape.m_radius = m_size;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.05f;
    fixtureDef.shape = &circleShape;
    m_body->CreateFixture(&fixtureDef);
}

ParticleElement::ParticleElement(Level* level, video::SColor color, core::vector2di pixPosition, unsigned life)
 : ParticleElement(level, color, core::vector2df(0,0), life)
{
    unsigned unit = m_level->getUnitSize();
    m_position.X = (float)pixPosition.X / (float)unit;
    m_position.Y = (float)pixPosition.Y / (float)unit;

    m_body->SetTransform({m_position.X, m_position.Y}, 0.f);
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
    unsigned unit = m_level->getUnitSize();
    core::vector2di pos(m_position.X * unit, m_position.Y * unit);
    pos -= m_level->getViewOffset();

    core::recti box(pos, pos);
    box.UpperLeftCorner -= m_size * unit;
    box.LowerRightCorner += m_size * unit;

    m_level->getGlobals()->driver->draw2DRectangle(m_color, box);
}