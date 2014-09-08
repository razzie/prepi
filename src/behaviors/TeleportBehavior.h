#ifndef TELEPORTBEHAVIOR_H_INCLUDED
#define TELEPORTBEHAVIOR_H_INCLUDED

#include <tuple>
#include <map>
#include "Behavior.h"

class TeleportBehavior : public Behavior
{
public:
    TeleportBehavior(Element*, std::istream&);
    TeleportBehavior(Element*, std::tuple<unsigned, float>);
    TeleportBehavior(Element*, unsigned sequenceNum, float randomness);
    ~TeleportBehavior();
    void setElement(Element*);
    void update(uint32_t elapsedMs);

protected:
    void activateNext();

    static std::map<unsigned, std::vector<TeleportBehavior*>> m_teleports;
    unsigned m_sequenceNum;
    float m_randomness;
};

#endif // TELEPORTBEHAVIOR_H_INCLUDED
