#ifndef PLAYERELEMENT_H_INCLUDED
#define PLAYERELEMENT_H_INCLUDED

#include <map>
#include "level\TileSet.h"
#include "Timer.h"
#include "elements\Element.h"

class EnemyElement;
class RewardElement;

class PlayerElement : public Element
{
public:
    PlayerElement(Level*, std::istream&);
    PlayerElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, float, float, BehaviorWrapper>);
    PlayerElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df, float, float, Behavior*);
    unsigned getHealth() const;
    unsigned getMaxHealth() const;
    unsigned getRewards() const;
    void takeHealth(unsigned);
    void takeReward(unsigned);
    void takeRewardFrom(RewardElement*);
    void takeDamage(unsigned);
    void takeDamageFrom(EnemyElement*);
    void die();
    void setSpeed(irr::f32);
    void setImmortal(uint32_t msec);
    void update(uint32_t elapsedMs);
    void draw();

protected:
    ~PlayerElement();

    unsigned m_health;
    unsigned m_rewards;
    irr::f32 m_speed;
    irr::core::vector2df m_lastVelocity;
    std::map<EnemyElement*, Timer> m_damageList;
    unsigned m_injury;
    uint32_t m_immortalLeft;
    TileData::Animation::Type m_lastAnimType;
    bool m_standbyAnim;
};

#endif // PLAYERELEMENT_H_INCLUDED
