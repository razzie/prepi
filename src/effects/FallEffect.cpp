#include "Globals.h"
#include "level\Level.h"
#include "effects\FallEffect.h"

using namespace irr;

FallEffect::FallEffect(Element* element)
 : ElementEffect(element)
 , m_elapsed(0)
{
}

FallEffect::~FallEffect()
{
}

void FallEffect::update(uint32_t elapsedMs)
{

}

bool FallEffect::isFinished() const
{
    return true;
}
