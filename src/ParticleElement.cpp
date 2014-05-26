#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Level.h"
#include "ParticleElement.h"

using namespace irr;

ParticleElement::ParticleElement(Level* level, video::SColor color, core::vector2di pixPosition)
 : Element(level, Type::PARTICLE, {0,0})
 , m_color(color)
 , m_elapsed(0)
{
    unsigned unit = m_level->getUnitSize();
    m_position.X = (float)pixPosition.X / (float)unit;
    m_position.Y = (float)pixPosition.Y / (float)unit;

    //m_boundingBox = core::rectf(-0.05f, -0.05f, 0.05f, 0.05f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(m_position.X, m_position.Y);
    bodyDef.fixedRotation = true; // do not rotate!
    bodyDef.userData = this;
    m_body = m_level->getPhysics()->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_p.Set(-1.f, -1.f);
    circleShape.m_radius = 0.05f;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.05f;
    fixtureDef.shape = &circleShape;
    m_body->CreateFixture(&fixtureDef);
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
    if (m_elapsed > 5000) this->remove(); // disappears after 5 seconds
}

void ParticleElement::draw()
{
    unsigned unit = m_level->getUnitSize();
    core::vector2di pos(m_position.X * unit, m_position.Y * unit);
    pos -= m_level->getViewOffset();

    core::recti box(pos, pos);
    box.UpperLeftCorner -= unit/20;
    box.LowerRightCorner += unit/20;

    //m_level->getGlobals()->driver->draw2DPolygon(pos, 3.f, m_color);
    m_level->getGlobals()->driver->draw2DRectangle(m_color, box);
}
