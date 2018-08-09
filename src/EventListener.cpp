/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "EventListener.h"

using namespace irr;

EventListener::EventListener()
 : m_leftMouseDown(false)
 , m_middleMouseDown(false)
 , m_rightMouseDown(false)
{
     for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
     {
         m_keyData[i].m_pressed = false;
         m_keyData[i].m_state = KeyState::UP;
     }
}

EventListener::~EventListener()
{
}

bool EventListener::OnEvent(const SEvent& event)
{
    switch (event.EventType)
    {
        case EET_KEY_INPUT_EVENT:
            {
                KeyData& key = m_keyData[event.KeyInput.Key];
                key.m_pressed = event.KeyInput.PressedDown;

                if (key.m_pressed)
                    key.m_state = KeyState::DOWN;
                else if (key.m_state == KeyState::DOWN)
                    key.m_state = KeyState::RELEASED;
            }
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
    return (m_keyData[keyCode].m_state == KeyState::DOWN);
}

bool EventListener::isKeyReleased(EKEY_CODE keyCode)
{
    KeyData& key = m_keyData[keyCode];
    if (key.m_state == KeyState::RELEASED)
    {
        key.m_state = KeyState::UP;
        return true;
    }
    else
    {
        return false;
    }
    //return (m_keyData[keyCode].m_state == KeyState::RELEASED);
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
