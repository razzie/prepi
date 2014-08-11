#include "Globals.h"
#include "level\Level.h"
#include "effects\DisappearEffect.h"

using namespace irr;

DisappearEffect::DisappearEffect(Element* element)
 : ElementEffect(element)
 , m_elapsed(0)
{
}

DisappearEffect::~DisappearEffect()
{
}

void DisappearEffect::update(uint32_t elapsedMs)
{

}

bool DisappearEffect::isFinished() const
{

}
