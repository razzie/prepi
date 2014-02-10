#ifndef ELEMENT_H
#define ELEMENT_H

#include "irrlicht.h"

class Level;

class Element
{
public:
    Element(Level*);
    virtual ~Element();

    irr::core::vector2di getPosition() const;
    void setPosition(irr::core::vector2di);

    virtual void update() = 0;
protected:
private:
    irr::core::vector2di m_position;
    Level* m_level;
};

#endif // ELEMENT_H
