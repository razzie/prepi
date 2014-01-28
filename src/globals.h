#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "irrlicht.h"
#include "Box2D\Box2D.h"
#include "EventListener.h"
#include <set>

class Element;

struct gameGlobals
{
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    EventListener* eventListener;
    b2World *world;
    std::set<Element*> elements;
};

#endif // GLOBALS_H_INCLUDED
