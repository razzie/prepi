#include "Globals.h"
#include "level\Level.h"
#include "effects\PickUpEffect.h"

using namespace irr;

PickUpEffect::PickUpEffect(Element* element)
 : ElementEffect(element)
 , m_elapsed(0)
{
}

PickUpEffect::~PickUpEffect()
{
}

void PickUpEffect::update(uint32_t elapsedMs)
{

}

bool PickUpEffect::isFinished() const
{

}
