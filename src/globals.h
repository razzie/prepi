#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "irrlicht.h"
#include "Box2D\Box2D.h"
//#include "EventListener.h"

class EventListener;

class Globals
{
public:
    irr::IrrlichtDevice* const device;
    irr::video::IVideoDriver* const driver;
    irr::scene::ISceneManager* const smgr;
    EventListener* const eventListener;
    b2World* const world;

    Globals();
    Globals(const Globals&) = delete;
    ~Globals();
};

#endif // GLOBALS_H_INCLUDED
