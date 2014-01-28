#include <iostream>
#include "globals.h"
#include "SFML\Audio.hpp"

using namespace std;
using namespace irr;
using namespace sf;

gameGlobals g;

int main()
{
    // create device
    g.device = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600), 16, false);
    if (g.device == 0)
        return 1; // could not create selected driver.

    g.driver = g.device->getVideoDriver();
    g.smgr = g.device->getSceneManager();
    g.eventListener = new EventListener();

    // create engine and camera
    g.device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
    g.smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
    g.device->setEventReceiver(g.eventListener);

    b2Vec2 gravity(0.0f, -1.0f);
    g.world = new b2World(gravity);

    SoundBuffer rofiBuf;
    rofiBuf.loadFromFile("../media/rofoges.wav");
    Sound rofi(rofiBuf);
    rofi.play();

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    u32 frames=0;

    while(g.device->run())
    {
        g.world->Step(timeStep, velocityIterations, positionIterations);

        g.driver->beginScene(true, true, video::SColor(0,100,100,100));

        //g.smgr->drawAll();





        g.driver->endScene();
        if (++frames==100)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += g.driver->getName();
            str += L"] FPS: ";
            str += (s32)g.driver->getFPS();

            g.device->setWindowCaption(str.c_str());
            frames=0;
        }
    }

    g.device->drop();

    return 0;
}
