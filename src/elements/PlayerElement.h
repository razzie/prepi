/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    bool dropElement(Element::Type, unsigned, irr::core::vector2di);
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
        uint32_t m_timeout;
        bool m_collided;
    };

    unsigned m_health;
    unsigned m_rewards;
    irr::f32 m_speed;
    bool m_checkpoint;
    irr::core::vector2df m_checkpointPos;
    unsigned m_checkpointHealth;
    irr::core::vector2df m_lastVelocity;
    irr::core::vector2df m_platformRelPosition;
    Element* m_movingPlatform;
    std::vector<Damage> m_damageList;
    unsigned m_injury;
    uint32_t m_immortalLeft;
    TileData::Animation::Type m_animType;
    TileData::Animation::Type m_lastAnimType;
    unsigned m_animShiftTimeout;
    unsigned m_animFrame;
    bool m_animStop;
    bool m_animRevert;
    bool m_lastDirectionLeft;
    bool m_onLadder;
};

#endif // PLAYERELEMENT_H_INCLUDED
