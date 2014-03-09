#include "Globals.h"
#include "EventListener.h"
#include "gg/application.hpp"

using namespace std;
using namespace irr;
//using namespace sf;

static IrrlichtDevice* createIrrlichtDevice()
{
    SIrrlichtCreationParameters params; // constructor sets the defaults
    params.AntiAlias = 16;
    params.Bits = 32;
    params.DriverType = video::EDT_OPENGL;
    params.Fullscreen = false;
    params.WindowSize = core::dimension2d<u32>(1366, 768);
    params.WithAlphaChannel = true;

    return createDeviceEx(params);
}

Globals::Globals()
 : device(createIrrlichtDevice())
 , driver(device->getVideoDriver())
 , smgr(device->getSceneManager())
 , eventListener(new EventListener())
 , app(gg::application::create("prepi"))
{
    device->setEventReceiver(eventListener);
    device->setResizable(true);
    device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
    smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));

    driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY);
    driver->enableMaterial2D(true);
    driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
    driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;
}

Globals::~Globals()
{
    device->drop();
    app->drop();
    delete eventListener;
}
