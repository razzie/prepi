#ifndef ELEMENT_H
#define ELEMENT_H

#include "globals.h"

class Element
{
    public:
        Element(gameGlobals*);
        virtual ~Element();

        irr::core::vector2di getPosition() const;
        void setPosition(irr::core::vector2di);

        virtual void update() = 0;
    protected:
    private:
        irr::core::vector2di m_position;
        gameGlobals* m_globals;
};

#endif // ELEMENT_H
