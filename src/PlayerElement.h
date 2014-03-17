#ifndef PLAYERELEMENT_H_INCLUDED
#define PLAYERELEMENT_H_INCLUDED

#include <map>
#include "Timer.h"
#include "Element.h"

class EnemyElement;
class RewardElement;

class PlayerElement : public Element
{
public:
    static constexpr irr::f32 NO_CLIMBING   = -0.9f;
    static constexpr irr::f32 HALF_CLIMBING = 0.0f;
    static constexpr irr::f32 FULL_CLIMBING = 0.9f;

    PlayerElement(Level*, std::istream&);
    PlayerElement(Level*, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df>);
    PlayerElement(Level*, unsigned, irr::core::vector2di, irr::core::vector2df);
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
    void setClimbingMode(irr::f32);
    void update();
    //void draw();

protected:
    ~PlayerElement();

    unsigned m_health;
    unsigned m_rewards;
    irr::f32 m_speed;
    irr::f32 m_climbTreshold;
    std::map<EnemyElement*, Timer> m_damageList;
};

#endif // PLAYERELEMENT_H_INCLUDED
