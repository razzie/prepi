#ifndef EXPLOSIVEBEHAVIOR_H_INCLUDED
#define EXPLOSIVEBEHAVIOR_H_INCLUDED

#include <tuple>
#include "Behavior.h"

class ExplosiveBehavior : public Behavior
{
public:
    ExplosiveBehavior(Element*, std::istream&);
    ExplosiveBehavior(Element*, std::tuple<unsigned, unsigned, float>);
    ExplosiveBehavior(Element*, unsigned damage, unsigned timeout, float range);
    ~ExplosiveBehavior();
    //void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    bool m_triggered;
    unsigned m_explosionTimer;
    irr::video::ITexture* texture;
    unsigned m_damage;
    unsigned m_timeout;
    unsigned m_range;
};

#endif // EXPLOSIVEBEHAVIOR_H_INCLUDED
