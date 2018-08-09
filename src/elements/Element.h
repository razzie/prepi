/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include <bitset>
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

    enum Flag
    {
        DRAW = 0,
        PHYSICS = 1,
        UPDATE = 2,
        FLAG_COUNT = 3 // do not use!
    };

    Element(Level*, Type, unsigned, irr::core::vector2di, irr::core::vector2df, float, float, Behavior*, Motion*);

    Level* getLevel();
    const Level* getLevel() const;

    Type getType() const;
    unsigned getId() const;
    irr::core::vector2di getImagePosition() const;

    bool getFlag(Flag) const;
    void setFlag(Flag, bool value = true);
    void setFlags(bool value = true);

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

    irr::core::vector2df getMovement() const;
    void setMovementX(irr::f32);
    void setMovementY(irr::f32);

    const TileData* getTileData() const;
    const Shape& getShape() const;
    irr::core::rectf getBoundingBox() const;
    b2Body* getBody();

    void updateCollisions();
    const std::vector<Collision>& getCollisions() const;
    bool isPlayerCollided();
    bool isPlayerCollided() const;

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
    std::bitset<Flag::FLAG_COUNT> m_flags;
    float m_scale;
    float m_animSpeed;
    Behavior* m_behavior;
    Motion* m_motion;
    Shape m_shape;
    const TileData* m_tileData;
    b2Body* m_body;
    std::vector<Collision> m_collisions;
};

Element* CreateElement(Level*, std::istream&);
Element* CreateElement(Level*, Element::Type, unsigned, irr::core::vector2di, irr::core::vector2df, bool dynamic = true);

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
