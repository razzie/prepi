#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include <vector>
#include "irrlicht.h"
#include "tinythread.h"
#include "Collision.h"
#include "Motion.h"
#include "Behavior.h"

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
        PARTICLE = 999
    };

    //Element(Level*, std::istream&);
    Element(Level*, Type, unsigned, irr::core::vector2di, irr::core::vector2df, float, Behavior*, Motion*);

    Level* getLevel();
    const Level* getLevel() const;

    Type getType() const;
    unsigned getId() const;
    irr::core::vector2di getImagePosition() const;

    irr::core::vector2df getPosition() const;
    void setPosition(irr::core::vector2df);

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

    irr::core::rectf getBoundingBox() const;
    b2Body* getBody();

    void updateCollisions();
    const std::vector<Collision>& getCollisions() const;

    void remove();

    virtual void update(uint32_t elapsedMs);

    virtual void draw();
    void drawDebugBox() const;

protected:
    Element(Level*, Type, irr::core::vector2df);
    virtual ~Element();

    mutable tthread::recursive_mutex m_mutex;
    Level* m_level;
    Type m_type;
    unsigned m_id;
    irr::core::vector2di m_imgPosition;
    irr::core::vector2df m_position;
    float m_animSpeed;
    Behavior* m_behavior;
    Motion* m_motion;
    irr::core::rectf m_boundingBox;
    const TileData* m_tileData;
    b2Body* m_body;
    std::vector<Collision> m_collisions;
};

Element* CreateElement(Level*, std::istream&);

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
