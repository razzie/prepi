#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>
#include <tuple>
#include "irrlicht.h"

class Level;

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
    irr::core::vector2di getPosition() const;
    irr::core::vector2df getRealCoord() const;
    void setRealCoord(irr::core::vector2df);
    virtual void update() = 0;

protected:
    Level* m_level;
    Type m_type;
    unsigned m_id;
    irr::core::vector2di m_position;
    irr::core::vector2df m_realCoord;
};

std::istream& operator>> (std::istream&, Element::Type&);
std::istream& operator>> (std::istream&, Element::Visibility&);
std::istream& operator>> (std::istream&, irr::core::vector2di&);
std::istream& operator>> (std::istream&, irr::core::vector2df&);

#endif // ELEMENT_H
