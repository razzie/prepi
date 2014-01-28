#include "EventListener.h"

using namespace irr;

EventListener::EventListener()
{
     for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
        KeyIsDown[i] = false;
}

bool EventListener::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT)
        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return true;
}

bool EventListener::IsKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

EventListener::~EventListener()
{
    //dtor
}
