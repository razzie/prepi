#ifndef APPEAREFFECT_H_INCLUDED
#define APPEAREFFECT_H_INCLUDED

#include "irrlicht.h"
#include "effects\ElementEffect.h"

class Element;

class AppearEffect : public ElementEffect
{
public:
    AppearEffect(Element*);
    ~AppearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    irr::core::vector2df m_pos;
    uint32_t m_elapsed;
    uint32_t m_duration;
    bool m_animation;
};

class DisappearEffect : public ElementEffect
{
public:
    DisappearEffect(Element*);
    ~DisappearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    irr::core::vector2df m_pos;
    uint32_t m_elapsed;
    uint32_t m_duration;
    bool m_animation;
};

class ReappearEffect : public ElementEffect
{
public:
    ReappearEffect(Element*, irr::core::vector2df newPos);
    ~ReappearEffect();
    void update(uint32_t elapsedMs);
    bool isFinished() const;

private:
    irr::core::vector2df m_pos1;
    irr::core::vector2df m_pos2;
    uint32_t m_elapsed;
    uint32_t m_duration1;
    uint32_t m_duration2;
    bool m_animation1;
    bool m_animation2;
};

#endif // APPEAREFFECT_H_INCLUDED
