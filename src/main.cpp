#include <iostream>
#include "Globals.h"
#include "SFML\Audio.hpp"

using namespace std;
using namespace irr;
using namespace sf;

int main()
{
    Globals g;

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

    return 0;
}
