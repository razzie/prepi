#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include "irrlicht.h"

class Level;
struct TileData;

class Element
{
public:
    enum class Type : unsigned
    {
        GROUND = 0,
        ENEMY = 1,
        REWARD = 2,
        PLAYER = 3,
        FINISH = 4
    };

    enum class Visibility : unsigned
    {
        BACK = 0,
        SOLID = 1,
        FORE = 2
    };

    Element(Level*, Type, unsigned, irr::core::vector2di, irr::core::vector2df);
    virtual ~Element();

    Type getType() const;
    unsigned getId() const;
    irr::core::vector2di getImagePosition() const;
    irr::core::vector2df getPosition() const;
    void setPosition(irr::core::vector2df);
    irr::core::recti getBoundingBox() const;
    virtual void update();
    virtual void draw();

protected:
    Level* m_level;
    Type m_type;
    unsigned m_id;
    irr::core::vector2di m_imgPosition;
    irr::core::vector2df m_position;
    irr::core::recti m_boudingBox;
    const TileData* m_tileData;

    static void drawTile(Level*, const TileData*, irr::core::vector2di imgPos, irr::core::vector2df pos);
};

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, Element::Visibility&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
