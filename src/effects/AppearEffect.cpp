#include "Globals.h"
#include "level\Level.h"
#include "effects\AppearEffect.h"

using namespace irr;

AppearEffect::AppearEffect(Element* element)
 : ElementEffect(element)
 , m_elapsed(0)
{
}

AppearEffect::~AppearEffect()
{
}

void AppearEffect::update(uint32_t elapsedMs)
{

}

bool AppearEffect::isFinished() const
{

}
