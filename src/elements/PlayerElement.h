#ifndef PLAYERELEMENT_H_INCLUDED
#define PLAYERELEMENT_H_INCLUDED

#include <vector>
#include "level\TileSet.h"
#include "elements\Element.h"

class EnemyElement;
class RewardElement;
class CheckpointBehavior;
class LadderBehavior;

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

    friend class CheckpointBehavior;
    friend class LadderBehavior;
    void checkpoint();
    void onLadder();

    struct Damage
    {
        EnemyElement* m_enemy;
        bool m_collided;
    };

    unsigned m_health;
    unsigned m_rewards;
    irr::f32 m_speed;
    bool m_checkpoint;
    irr::core::vector2df m_checkpointPos;
    unsigned m_checkpointHealth;
    irr::core::vector2df m_lastVelocity;
    std::vector<Damage> m_damageList;
    unsigned m_injury;
    uint32_t m_immortalLeft;
    TileData::Animation::Type m_lastAnimType;
    bool m_onLadder;
    bool m_lastDirectionLeft;
};

#endif // PLAYERELEMENT_H_INCLUDED
