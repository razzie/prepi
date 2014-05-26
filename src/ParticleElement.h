#ifndef PARTICLEELEMENT_H_INCLUDED
#define PARTICLEELEMENT_H_INCLUDED

#include "irrlicht.h"
#include "Element.h"

class ParticleElement : public Element
{
public:
    ParticleElement(Level*, irr::video::SColor, irr::core::vector2di);
    ~ParticleElement();
    irr::video::SColor getColor() const;
    void setColor(irr::video::SColor);
    virtual void update(uint32_t elapsedMs);
    virtual void draw();

private:
    irr::video::SColor m_color;
    uint32_t m_elapsed;
};

#endif // PARTICLEELEMENT_H_INCLUDED
