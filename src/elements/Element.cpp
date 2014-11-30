#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "level\TileSet.h"
#include "effects\EffectManager.h"
#include "elements\Element.h"
#include "elements\GroundElement.h"
#include "elements\EnemyElement.h"
#include "elements\RewardElement.h"
#include "elements\PlayerElement.h"
#include "elements\FinishElement.h"
#include "elements\ParticleElement.h"

using namespace irr;

void getLine(std::istream& input, std::ostream& output)
{
    while (input && input.peek() != '\n')
    {
        output << (char)input.get();
    }
}

Element* CreateElement(Level* level, std::istream& input)
{
    std::stringstream stream;
    getLine(input, stream);

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

Element* CreateElement(Level* level, Element::Type type, unsigned id, core::vector2di imgPos, core::vector2df pos, bool dynamic)
{
    Motion* motion = nullptr;

    switch (type)
    {
        case Element::Type::GROUND:
            if (dynamic) motion = new Motion(nullptr, Motion::Type::DYNAMIC);
            return new GroundElement(level, id, imgPos, pos, 1.f, 1.f, nullptr, motion);

        case Element::Type::ENEMY:
            if (dynamic) motion = new Motion(nullptr, Motion::Type::DYNAMIC);
            return new EnemyElement(level, id, imgPos, pos, 1.f, 1.f, nullptr, motion, 10);

        case Element::Type::REWARD:
            if (dynamic) motion = new Motion(nullptr, Motion::Type::DYNAMIC);
            return new RewardElement(level, id, imgPos, pos, 1.f, 1.f, nullptr, motion, 10);

        case Element::Type::PLAYER:
            return new PlayerElement(level, id, imgPos, pos, 1.f, 1.f, nullptr);

        case Element::Type::FINISH:
            return new FinishElement(level, id, imgPos, pos, 1.f, 1.f, nullptr);

        case Element::Type::PARTICLE:
            return new ParticleElement(level, id, pos, 1.f);

        default:
            return nullptr;
    }
}

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, core::vector2df position,
                 float scale, float animSpeed, Behavior* behavior, Motion* motion)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_imgPosition(imgPosition)
 , m_position(position)
 , m_scale(scale)
 , m_animSpeed((animSpeed < 0.1f) ? 0.1f : animSpeed)
 , m_behavior(behavior)
 , m_motion(motion)
 , m_tileData(level->getTileSet()->getData(type, id))
 , m_body(nullptr)
{
    m_flags.set();

    if (m_tileData != nullptr)
    {
        m_shape = m_tileData->getBoundingShape(imgPosition);
        m_shape *= m_scale;
        m_body = m_tileData->createBody(this);

        if (m_tileData->getAnimation(m_imgPosition, TileData::Animation::Type::APPEAR) != nullptr &&
            getBehaviorType() != Behavior::Type::TELEPORT)
        {
            m_level->getEffectManager()->appear(this);
        }
    }

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
 , m_imgPosition(0, 0)
 , m_position(position)
 , m_scale(1.0f)
 , m_animSpeed(1.0f)
 , m_behavior(nullptr)
 , m_motion(nullptr)
 , m_tileData(nullptr)
 , m_body(nullptr)
{
    m_flags.set();

    m_level->addElement(this);
}

Element::~Element()
{
    if (m_body != nullptr)
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

bool Element::getFlag(Flag flag) const
{
    return m_flags.test(flag);
}

void Element::setFlag(Flag flag, bool value)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    m_flags.set(flag, value);

    if (flag == Flag::PHYSICS && m_body != nullptr)
    {
        m_body->SetActive(value);
    }
}

void Element::setFlags(bool value)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (value)
        m_flags.set();
    else
        m_flags.reset();

    if (m_body != nullptr)
    {
        m_body->SetActive(value);
    }
}

core::vector2df Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(core::vector2df position)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    m_position = position;

    if (m_body != nullptr)
    {
        m_body->SetTransform({position.X, position.Y}, 0.f);
        m_body->SetLinearVelocity({0.f, 0.f});
    }
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
        m_shape = m_tileData->getBoundingShape(m_imgPosition);
        m_shape *= scale;
    }
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

core::vector2df Element::getMovement() const
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_body == nullptr)
    {
        return {0.f, 0.f};
    }
    else
    {
        b2Vec2 movement = m_body->GetLinearVelocity();
        return {movement.x, movement.y};
    }
}

void Element::setMovementX(f32 xMov)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_body != nullptr)
    {
        f32 y = m_body->GetLinearVelocity().y;
        m_body->SetLinearVelocity({xMov, y});
    }
}

void Element::setMovementY(f32 yMov)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_body != nullptr)
    {
        f32 x = m_body->GetLinearVelocity().x;
        m_body->SetLinearVelocity({x, yMov});
    }
}

const TileData* Element::getTileData() const
{
    return m_tileData;
}

const Shape& Element::getShape() const
{
    return m_shape;
}

core::rectf Element::getBoundingBox() const
{
    return m_shape.getBoxData();
}

b2Body* Element::getBody()
{
    return m_body;
}

void Element::updateCollisions()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);
    Collision::updateElementCollisions(this, m_collisions);
}

const std::vector<Collision>& Element::getCollisions() const
{
    return m_collisions;
}

bool Element::isPlayerCollided()
{
    updateCollisions();

    // calling const version of this function
    return (const_cast<const Element*>(this)->isPlayerCollided());
}

bool Element::isPlayerCollided() const
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    // there is no box2d body, checking collision based on bounding boxes
    if (m_body == nullptr)
    {
        PlayerElement* player = m_level->getPlayerElement();

        if (player == nullptr) return false;

        core::rectf playerBox = player->getBoundingBox() + player->m_position;
        core::rectf elementBox = getBoundingBox() + m_position;

        return (elementBox.isRectCollided(playerBox));
    }
    // retrieving box2d collisions (more accurate)
    else
    {
        for (auto collision : m_collisions)
        {
            Element* contactElem = collision.getElementB();

            if (contactElem->getType() == Element::Type::PLAYER)
            {
                return true;
            }
        }

        return false;
    }
}

void Element::remove()
{
    m_level->removeElement(this);
}

void Element::update(uint32_t elapsedMs)
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_body != nullptr)
    {
        b2Vec2 pos = m_body->GetPosition();
        m_position.X = pos.x;
        m_position.Y = pos.y;
    }

    if (m_motion != nullptr)
    {
        m_motion->update(elapsedMs);
    }

    if (m_behavior != nullptr)
    {
        m_behavior->update(elapsedMs);
    }
}

void Element::draw()
{
    tthread::lock_guard<tthread::recursive_mutex> guard(m_mutex);

    if (m_tileData != nullptr)
    {
        m_tileData->drawAnimation(m_level, m_imgPosition, 0, m_animSpeed, m_position, m_scale);
    }
    else
    {
        m_shape.draw(m_level, m_position);
    }
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
