/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef TELEPORTBEHAVIOR_H_INCLUDED
#define TELEPORTBEHAVIOR_H_INCLUDED

#include <tuple>
#include <map>
#include "behaviors\Behavior.h"

class TeleportBehavior : public Behavior
{
public:
    TeleportBehavior(Element*, std::istream&);
    TeleportBehavior(Element*, std::tuple<unsigned, float, int>);
    TeleportBehavior(Element*, unsigned sequenceNum, float randomness, int delay);
    ~TeleportBehavior();
    void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    void activateNext();

    static std::map<unsigned, std::vector<TeleportBehavior*>> m_teleports;
    unsigned m_sequenceNum;
    float m_randomness;
    int m_delay;
    bool m_touched;
    unsigned m_untouchTimer;
};

#endif // TELEPORTBEHAVIOR_H_INCLUDED
