/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "Globals.h"
#include "Parser.h"
#include "level\Level.h"
#include "effects\EffectManager.h"
#include "elements\RewardElement.h"

using namespace irr;

RewardElement::RewardElement(Level* level, std::istream& stream)
 : RewardElement(level, Parser(stream, ';').getArgs<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                    float, float, BehaviorWrapper, MotionWrapper, unsigned>())
{
}

RewardElement::RewardElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df,
                                                      float, float, BehaviorWrapper, MotionWrapper, unsigned> data)
 : RewardElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data),
    std::get<5>(data).behavior,
    std::get<6>(data).motion,
    std::get<7>(data))
{
}

RewardElement::RewardElement(Level* level, unsigned id,
                             irr::core::vector2di imgPosition, core::vector2df position,
                             float scale, float animSpeed, Behavior* behavior, Motion* motion, unsigned value)
 : Element(level, Type::REWARD, id, imgPosition, position, scale, animSpeed, behavior, motion)
 , m_value(value)
{
}

RewardElement::~RewardElement()
{
    m_level->getEffectManager()->pickUp(this);
}

unsigned RewardElement::getValue() const
{
    return m_value;
}
