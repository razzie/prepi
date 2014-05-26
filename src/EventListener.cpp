#include "EventListener.h"

using namespace irr;

EventListener::EventListener()
 : m_leftMouseDown(false)
 , m_middleMouseDown(false)
 , m_rightMouseDown(false)
{
     for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
        m_keyDown[i] = false;
}

EventListener::~EventListener()
{
}

bool EventListener::OnEvent(const SEvent& event)
{
    switch (event.EventType)
    {
        case EET_KEY_INPUT_EVENT:
            m_keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            break;

        case EET_MOUSE_INPUT_EVENT:
            m_leftMouseDown = event.MouseInput.isLeftPressed();
            m_middleMouseDown = event.MouseInput.isMiddlePressed();
            m_rightMouseDown = event.MouseInput.isRightPressed();
            m_mousePos.X = event.MouseInput.X;
            m_mousePos.Y = event.MouseInput.Y;
            break;

        default:
            break;
    }

    return true;
}

bool EventListener::isLeftMouseDown() const
{
    return m_leftMouseDown;
}

bool EventListener::isMiddleMouseDown() const
{
    return m_middleMouseDown;
}

bool EventListener::isRightMouseDown() const
{
    return m_rightMouseDown;
}

core::vector2di EventListener::getMousePosition() const
{
    return m_mousePos;
}

bool EventListener::isKeyDown(EKEY_CODE keyCode) const
{
    return m_keyDown[keyCode];
}

bool EventListener::isUp() const
{
    return (isKeyDown(KEY_KEY_W) || isKeyDown(KEY_UP));
}

bool EventListener::isDown() const
{
    return (isKeyDown(KEY_KEY_S) || isKeyDown(KEY_DOWN));
}

bool EventListener::isLeft() const
{
    return (isKeyDown(KEY_KEY_A) || isKeyDown(KEY_LEFT));
}

bool EventListener::isRight() const
{
    return (isKeyDown(KEY_KEY_D) || isKeyDown(KEY_RIGHT));
}
