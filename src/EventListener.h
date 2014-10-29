#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "irrlicht.h"

class EventListener : public irr::IEventReceiver
{
    public:
        EventListener();
        virtual ~EventListener();
        virtual bool OnEvent(const irr::SEvent&);
        bool isLeftMouseDown() const;
        bool isMiddleMouseDown() const;
        bool isRightMouseDown() const;
        irr::core::vector2di getMousePosition() const;
        bool isKeyDown(irr::EKEY_CODE) const;
        bool isKeyReleased(irr::EKEY_CODE);
        bool isUp() const;
        bool isDown() const;
        bool isLeft() const;
        bool isRight() const;

    protected:
    private:
        enum KeyState
        {
            UP,
            DOWN,
            RELEASED
        };

        struct KeyData
        {
            bool m_pressed;
            KeyState m_state;
        };

        KeyData m_keyData[irr::KEY_KEY_CODES_COUNT];
        bool m_leftMouseDown;
        bool m_middleMouseDown;
        bool m_rightMouseDown;
        irr::core::vector2di m_mousePos;
};

#endif // EVENTLISTENER_H
