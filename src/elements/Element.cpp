#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "elements\Element.h"
#include "elements\GroundElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"
#include "elements\PlayerElement.h"
#include "elements\FinishElement.h"

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

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, core::vector2df position,
                 float animSpeed, Behavior* behavior, Motion* motion)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_imgPosition(imgPosition)
 , m_position(position)
 , m_scale(1.0f)
 , m_animSpeed((animSpeed < 0.1f) ? 0.1f : animSpeed)
 , m_behavior(behavior)
 , m_motion(motion)
 , m_tileData(level->getTileSet()->getData(type, id))
// , m_body(nullptr)
 , m_enabled(true)
{
    m_boundingBox = m_tileData->getBoundingShape(imgPosition).getBoxData();

    if (m_tileData != nullptr)
        m_body = m_tileData->createBody(this);

    m_level->addElement(this);

    if (m_motion != nullptr && m_motion->getElement() == nullptr)
        m_motion->setElement(this);

    if (m_behavior != nullptr && m_behavior->getElement() == nullptr)
        m_behavior->setElement(this);
}

Element::Element(Level* level, Type type, core::vector2df position)
 : m_level(level)
 , m_type(type)
 , m_id(0)
// , m_imgPosition({0,0})
 , m_position(position)
 , m_scale(1.0f)
 , m_animSpeed(1.0f)
 , m_behavior(nullptr)
 , m_motion(nullptr)
 , m_tileData(nullptr)
 , m_body(nullptr)
 , m_enabled(true)
{
    m_level->addElement(this);
}

Element::~Element()
{
    if (m_body)
    {
        m_level->getPhysics()->DestroyBody(m_body);
        m_body = nullptr;
    }

    if (m_motion != nullptr) delete m_motion;

    if (m_behavior != nullptr) delete m_behavior;
}

Level* Element::getLevel()
{
    return m_level;
}

const Level* Element::getLevel() const
{
    return m_level;
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

float Element::getScale() const
{
    return m_scale;
}

void Element::setScale(float scale)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (scale < 0.f) scale *= -1.f;
    m_scale = scale;

    if (m_body != nullptr && m_tileData != nullptr)
    {
        m_level->getPhysics()->DestroyBody(m_body);
        m_body = m_tileData->createBody(this);
    }
}

void Element::setPosition(core::vector2df position)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_position = position;
    m_body->SetTransform({position.X, position.Y}, 0.f);
    m_body->SetLinearVelocity({0.f, 0.f});
}

float Element::getAnimSpeed() const
{
    return m_animSpeed;
}

void Element::setAnimSpeed(float animSpeed)
{
    m_animSpeed = (animSpeed < 0.1f) ? 0.1f : animSpeed;
}

Behavior* Element::getBehavior()
{
    return m_behavior;
}

const Behavior* Element::getBehavior() const
{
    return m_behavior;
}

Behavior::Type Element::getBehaviorType() const
{
    if (m_behavior == nullptr)
        return Behavior::Type::NONE;
    else
        return m_behavior->getType();
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

const TileData* Element::getTileData() const
{
    return m_tileData;
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

core::rectf Element::getBoundingBox() const
{
    return m_boundingBox;
}

b2Body* Element::getBody()
{
    return m_body;
}

void Element::updateCollisions()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    Collision::getElementCollisions(this, m_collisions);
}

const std::vector<Collision>& Element::getCollisions() const
{
    return m_collisions;
}

void Element::enable(bool enabled)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    m_body->SetActive(enabled);
    m_enabled = enabled;
}

bool Element::isEnabled() const
{
    return m_enabled;
}

void Element::remove()
{
    m_level->removeElement(this);
}

void Element::update(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_body)
    {
        b2Vec2 pos = m_body->GetPosition();
        m_position.X = pos.x;
        m_position.Y = pos.y;
    }

    if (m_motion)
    {
        m_motion->update(elapsedMs);
    }

    if (m_behavior)
    {
        m_behavior->update(elapsedMs);
    }
}

void Element::draw()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_tileData)
        m_tileData->drawTile(m_level, m_imgPosition, m_position, m_scale);
    else
        drawDebugBox();
}

void Element::drawDebugBox() const
{
    unsigned unit = m_level->getUnitSize();
    core::rectf box(m_boundingBox.UpperLeftCorner * m_scale, m_boundingBox.LowerRightCorner * m_scale);
    box += m_position;
    //core::rectf box = m_boundingBox + m_position;
    core::recti pixelBox( (s32)(box.UpperLeftCorner.X * unit),  (s32)(box.UpperLeftCorner.Y * unit),
                          (s32)(box.LowerRightCorner.X * unit), (s32)(box.LowerRightCorner.Y * unit) );
    pixelBox -= m_level->getViewOffset();

    m_level->getGlobals()->driver->draw2DRectangleOutline(pixelBox);
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
