#ifndef EFFECTMANAGER_H_INCLUDED
#define EFFECTMANAGER_H_INCLUDED

#include <cstdint>
#include <vector>
#include "irrlicht.h"
#include "tinythread.h"

class Level;
class Element;

class Effect // interface class for complex effects
{
public:
    virtual ~Effect() {}
    virtual void update(uint32_t elapsedMs) = 0;
    virtual bool isFinished() const = 0;
};

class EffectManager
{
public:
    EffectManager(Level*);
    ~EffectManager();
    void addEffect(Effect*); // do not use effect pointer after that
    void update(uint32_t elapsedMs);

    void playerDamage();
    void explosion(irr::core::vector2df pos, float range,
                   irr::video::SColor color = irr::video::SColor(255, 255, 0, 0));
    void appear(Element*);
    void disappear(Element*);
    void fall(Element*);
    void pickUp(Element*);

protected:
    mutable tthread::recursive_mutex m_mutex;
    Level* m_level;
    std::vector<Effect*> m_effects;
};

#endif // EFFECTMANAGER_H_INCLUDED
