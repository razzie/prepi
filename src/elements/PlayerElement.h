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
    static constexpr irr::f32 NO_CLIMBING   = 0.0f;
    static constexpr irr::f32 HALF_CLIMBING = 30.0f;
    static constexpr irr::f32 FULL_CLIMBING = 60.0f;

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
    void setClimbingMode(irr::f32);
    void update(uint32_t elapsedMs);
    void draw();

protected:
    ~PlayerElement();

    unsigned m_health;
    unsigned m_rewards;
    irr::f32 m_speed;
    irr::f32 m_climbTreshold;
    irr::core::vector2df m_prevVelocity;
    std::map<EnemyElement*, Timer> m_damageList;
    TileData::Animation::Type m_lastAnimType;
    bool m_standbyAnim;
};

#endif // PLAYERELEMENT_H_INCLUDED
