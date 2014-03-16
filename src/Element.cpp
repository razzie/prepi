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
    Element::Type type = Parser(stream).getArg<Element::Type>();

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

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, core::vector2df position, Motion motion)
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

    if (m_type != Type::GROUND) m_motion = Motion::DYNAMIC; // why do everything have static motion??

    b2BodyDef bodyDef;
    switch (m_motion)
    {
        case Motion::DYNAMIC:
            bodyDef.type = b2_dynamicBody;
            break;

        case Motion::AUTO:
        case Motion::STRAIGHT:
        case Motion::CIRCULAR:
            bodyDef.type = b2_kinematicBody;
            break;

        case Motion::STATIC:
        default:
            bodyDef.type = b2_staticBody;
            break;
    }
    bodyDef.position.Set(position.X, position.Y);
    bodyDef.fixedRotation = true; // do not rotate!
    bodyDef.userData = this;
    m_body = level->getPhysics()->CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(tileBBox.getWidth()/2, tileBBox.getHeight()/2,
                      {+tileBBox.getWidth()/2 - (1.f - tileBBox.UpperLeftCorner.X), +tileBBox.getHeight()/2 - (1.f - tileBBox.UpperLeftCorner.Y)},
                      0.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    m_body->CreateFixture(&fixtureDef);

    m_level->addElement(this);
}

Element::~Element()
{
    m_level->getPhysics()->DestroyBody(m_body);
    m_body = nullptr;
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
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    m_position = position;
    m_body->SetTransform({position.X, position.Y}, 0.f);
    m_body->SetLinearVelocity({0.f, 0.f});
}

Motion Element::getMotion() const
{
    return m_motion;
}

void Element::setMovementX(f32 xMov)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    f32 y = m_body->GetLinearVelocity().y;
    m_body->SetLinearVelocity({xMov, y});
}

void Element::setMovementY(f32 yMov)
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    f32 x = m_body->GetLinearVelocity().x;
    m_body->SetLinearVelocity({x, yMov});
}

core::recti Element::getBoundingBox() const
{
    return m_boundingBox;
}

void Element::update()
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
    b2Vec2 pos = m_body->GetPosition();
    m_position.X = pos.x;
    m_position.Y = pos.y;
}

void Element::draw()
{
    tthread::lock_guard<tthread::mutex> guard(m_mutex);
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

std::istream& operator>> (std::istream& i, Element::Visibility& v)
{
    unsigned visibility;
    i >> visibility;
    v = static_cast<Element::Visibility>(visibility);
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2di& v)
{
    int x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2df& v)
{
    float x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}
