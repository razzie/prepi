#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include <vector>
#include "irrlicht.h"
#include "tinythread.h"
#include "Shape.h"
#include "Collision.h"
#include "motions\Motion.h"
#include "behaviors\Behavior.h"

class Level;
class TileData;
class b2Body;

class Element
{
public:
    friend class Level;
    friend class Motion;
    friend class Behavior;

    enum class Type : unsigned
    {
        GROUND = 0,
        ENEMY = 1,
        REWARD = 2,
        PLAYER = 3,
        FINISH = 4,
        PARTICLE = 5
    };

    //Element(Level*, std::istream&);
    Element(Level*, Type, unsigned, irr::core::vector2di, irr::core::vector2df, float, float, Behavior*, Motion*);

    Level* getLevel();
    const Level* getLevel() const;

    Type getType() const;
    unsigned getId() const;
    irr::core::vector2di getImagePosition() const;

    irr::core::vector2df getPosition() const;
    void setPosition(irr::core::vector2df);

    float getScale() const;
    void setScale(float);

    float getAnimSpeed() const;
    void setAnimSpeed(float);

    Behavior* getBehavior();
    const Behavior* getBehavior() const;
    Behavior::Type getBehaviorType() const;

    Motion* getMotion();
    const Motion* getMotion() const;
    Motion::Type getMotionType() const;

    const TileData* getTileData() const;

    void setMovementX(irr::f32);
    void setMovementY(irr::f32);

    const Shape& getShape() const;
    irr::core::rectf getBoundingBox() const;
    b2Body* getBody();

    void updateCollisions();
    const std::vector<Collision>& getCollisions() const;

    void enable(bool enabled = true);
    bool isEnabled() const;
    void remove();

    virtual void update(uint32_t elapsedMs);
    virtual void draw();

protected:
    Element(Level*, Type, irr::core::vector2df);
    virtual ~Element();

    mutable tthread::recursive_mutex m_mutex;
    Level* m_level;
    Type m_type;
    unsigned m_id;
    irr::core::vector2di m_imgPosition;
    irr::core::vector2df m_position;
    float m_scale;
    float m_animSpeed;
    Behavior* m_behavior;
    Motion* m_motion;
    Shape m_shape;
    const TileData* m_tileData;
    b2Body* m_body;
    std::vector<Collision> m_collisions;
    bool m_enabled;
};

Element* CreateElement(Level*, std::istream&);

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
