#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H
#include <irrlicht.h>

class EventListener : public irr::IEventReceiver
{
    public:
        EventListener();
        virtual bool OnEvent(const irr::SEvent&);
        virtual bool IsKeyDown(irr::EKEY_CODE) const;
        virtual ~EventListener();
    protected:
    private:
        bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
};

#endif // EVENTLISTENER_H
