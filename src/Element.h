#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include <vector>
#include "irrlicht.h"
#include "tinythread.h"
#include "Collision.h"
#include "Motion.h"

class Level;
class TileData;
class b2Body;

class Element
{
public:
    friend class Level;
    friend class Motion;

    enum class Type : unsigned
    {
        GROUND = 0,
        ENEMY = 1,
        REWARD = 2,
        PLAYER = 3,
        FINISH = 4
    };

    //Element(Level*, std::istream&);
    Element(Level*, Type, unsigned, irr::core::vector2di, irr::core::vector2df, Motion*);
    Type getType() const;
    unsigned getId() const;
    irr::core::vector2di getImagePosition() const;
    irr::core::vector2df getPosition() const;
    void setPosition(irr::core::vector2df);
    Motion* getMotion();
    const Motion* getMotion() const;
    Motion::Type getMotionType() const;
    void setMovementX(irr::f32);
    void setMovementY(irr::f32);
    irr::core::recti getBoundingBox() const;
    b2Body* getBody();
    const std::vector<Collision> getCollisions() const;
    std::vector<Collision> getCollisionsForUpdate();
    void remove();
    virtual void update(uint32_t elapsedMs);
    virtual void draw();

protected:
    virtual ~Element();

    mutable tthread::recursive_mutex m_mutex;
    Level* m_level;
    Type m_type;
    unsigned m_id;
    irr::core::vector2di m_imgPosition;
    irr::core::vector2df m_position;
    Motion* m_motion;
    irr::core::recti m_boundingBox;
    const TileData* m_tileData;
    b2Body* m_body;
    std::vector<Collision> m_collisions;

    static void drawTile(Level*, const TileData*, irr::core::vector2di imgPos, irr::core::vector2df pos);
};

Element* CreateElement(Level*, std::istream&);

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
