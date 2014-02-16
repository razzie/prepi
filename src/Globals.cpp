#include "Globals.h"
#include "EventListener.h"
#include "gg/application.hpp"

using namespace std;
using namespace irr;
//using namespace sf;

static const b2Vec2 gravity(0.0f, -1.0f);

Globals::Globals()
 : device(createDevice(video::EDT_OPENGL, core::dimension2du(800, 600), 16, false))
 , driver(device->getVideoDriver())
 , smgr(device->getSceneManager())
 , eventListener(new EventListener())
 , world(new b2World(gravity))
 , app(gg::application::create("prepi"))
{
    // create engine and camera
    device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
    smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
    device->setEventReceiver(eventListener);
}

Globals::~Globals()
{
    device->drop();
    app->drop();
    delete world;
    delete eventListener;
}
