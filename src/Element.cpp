#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "TileSet.h"
#include "Element.h"
#include "GroundElement.h"
#include "EnemyElement.h"
#include "RewardElement.h"
#include "PlayerElement.h"
#include "FinishElement.h"

using namespace irr;

Element* CreateElement(Level* level, std::istream& stream)
{
    Element::Type type = Parser(stream, ';').getArg<Element::Type>();

    switch (type)
    {
        case Element::Type::GROUND:
            return new GroundElement(level, stream);

        case Element::Type::ENEMY:
            return new EnemyElement(level, stream);

        case Element::Type::REWARD:
            return new RewardElement(level, stream);

        case Element::Type::PLAYER:
            return new PlayerElement(level, stream);

        case Element::Type::FINISH:
            return new FinishElement(level, stream);

        default:
            return nullptr;
    }
}

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, core::vector2df position, Motion* motion)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_imgPosition(imgPosition)
 , m_position(position)
 , m_motion(motion)
 , m_tileData(level->getTileSet()->getData(type, id))
{
    s32 unit = (s32)m_level->getUnitSize();
    core::rectf tileBBox = m_tileData->getBoundingBox(imgPosition);
    m_boundingBox = {
        (s32)(tileBBox.UpperLeftCorner.X * unit), (s32)(tileBBox.UpperLeftCorner.Y * unit),
        (s32)(tileBBox.LowerRightCorner.X * unit), (s32)(tileBBox.LowerRightCorner.Y * unit)};

    if (getMotionType() != Motion::Type::NONE)
    {
        b2BodyDef bodyDef;
        switch (m_motion->getType())
        {
            case Motion::Type::DYNAMIC:
                bodyDef.type = b2_dynamicBody;
                break;

            case Motion::Type::AUTO:
            case Motion::Type::STRAIGHT:
            case Motion::Type::CIRCULAR:
                bodyDef.type = b2_kinematicBody;
                break;

            case Motion::Type::STATIC:
            case Motion::Type::UNSTABLE:
            default:
                bodyDef.type = b2_staticBody;
                break;
        }
        bodyDef.position.Set(position.X, position.Y);
        bodyDef.fixedRotation = true; // do not rotate!
        bodyDef.userData = this;
        m_body = level->getPhysics()->CreateBody(&bodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(tileBBox.getWidth()/2 - 0.01f, tileBBox.getHeight()/2 - 0.01f,
                          {+tileBBox.getWidth()/2 - (1.f - tileBBox.UpperLeftCorner.X), +tileBBox.getHeight()/2 - (1.f - tileBBox.UpperLeftCorner.Y)},
                          0.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.5f;
        m_body->CreateFixture(&fixtureDef);
    }

    m_level->addElement(this);
}

Element::~Element()
{
    m_level->getPhysics()->DestroyBody(m_body);
    m_body = nullptr;
    if (m_motion != nullptr) delete m_motion;
}

Element::Type Element::getType() const
{
    return m_type;
}

unsigned Element::getId() const
{
    return m_id;
}

core::vector2di Element::getImagePosition() const
{
    return m_imgPosition;
}

core::vector2df Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(core::vector2df position)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_position = position;
    m_body->SetTransform({position.X, position.Y}, 0.f);
    m_body->SetLinearVelocity({0.f, 0.f});
}

Motion* Element::getMotion()
{
    return m_motion;
}

const Motion* Element::getMotion() const
{
    return m_motion;
}

Motion::Type Element::getMotionType() const
{
    if (m_motion == nullptr)
        return Motion::Type::NONE;
    else
        return m_motion->getType();
}

void Element::setMovementX(f32 xMov)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    f32 y = m_body->GetLinearVelocity().y;
    m_body->SetLinearVelocity({xMov, y});
}

void Element::setMovementY(f32 yMov)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    f32 x = m_body->GetLinearVelocity().x;
    m_body->SetLinearVelocity({x, yMov});
}

core::recti Element::getBoundingBox() const
{
    return m_boundingBox;
}

b2Body* Element::getBody()
{
    return m_body;
}

const std::vector<Collision> Element::getCollisions() const
{
    return m_collisions;
}

std::vector<Collision> Element::getCollisionsForUpdate()
{
    return m_collisions;
}

void Element::remove()
{
    m_level->removeElement(this);
}

void Element::update(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    b2Vec2 pos = m_body->GetPosition();
    m_position.X = pos.x;
    m_position.Y = pos.y;

    if (m_motion)
    {
        m_motion->update(elapsedMs);
    }
}

void Element::draw()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    drawTile(m_level, m_tileData, m_imgPosition, m_position);
}

void Element::drawTile(Level* level, const TileData* td, core::vector2di imgPos, core::vector2df pos)
{
    core::rect<s32> srcRect =
        {(s32)(imgPos.X * td->tileSize), (s32)(imgPos.Y * td->tileSize),
        (s32)((imgPos.X + 1) * td->tileSize), (s32)((imgPos.Y + 1) * td->tileSize)};

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    core::rect<s32> destRect = {0, 0, (s32)unit, (s32)unit};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    level->getGlobals()->driver->draw2DImage(td->texture, destRect, srcRect, 0, 0, true);
}


std::istream& operator>> (std::istream& i, Element::Type& t)
{
    unsigned type;
    i >> type;
    t = static_cast<Element::Type>(type);
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2di& v)
{
    i >> v.X;
    i.ignore();
    i >> v.Y;
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2df& v)
{
    i >> v.X;
    i.ignore();
    i >> v.Y;
    return i;
}
