#include "effects\DelayEffect.h"

DelayEffect::DelayEffect(uint32_t delay, Effect* delayedEffect)
 : m_delay(delay)
 , m_elapsed(0)
 , m_delayedEffect(delayedEffect)
{
}

DelayEffect::~DelayEffect()
{
    if (m_delayedEffect) delete m_delayedEffect;
}

void DelayEffect::update(uint32_t elapsedMs)
{
    if (m_elapsed > m_delay && m_delayedEffect)
    {
        m_delayedEffect->update(elapsedMs);
    }

    m_elapsed += elapsedMs;
}

bool DelayEffect::isFinished() const
{
    if (m_elapsed <= m_delay)
    {
        return false;
    }
    else if (m_delayedEffect)
    {
        return m_delayedEffect->isFinished();
    }
    else
        return true;
}
