#include <iostream>
#include "Globals.h"
#include "Element.h"
#include "Level.h"
#include "SFML\Audio.hpp"
#include "gglib.hpp"

using namespace std;
using namespace irr;
using namespace sf;

int main()
{
    Globals g;

    /*SoundBuffer rofiBuf;
    rofiBuf.loadFromFile("../media/rofoges.wav");
    Sound rofi(rofiBuf);
    rofi.play();*/

    Level level1(&g, "../levels/mentes.txt");

    gg::script_engine* se = g.app->get_script_engine();
    se->add_function("setBackground", [&](unsigned id, unsigned mode)
                     { level1.setBackground(id, static_cast<Level::bgDrawingMethod>(mode)); });
    se->add_function("setDimension", [&](unsigned columns, unsigned rows)
                     { level1.setDimension(columns, rows); });

    gg::console* con = g.app->create_console();
    con->open();
    con->drop();

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    u32 frames=0;

    while(g.device->run())
    {
        g.world->Step(timeStep, velocityIterations, positionIterations);

        g.driver->beginScene(true, true, video::SColor(0,100,100,100));

        //g.smgr->drawAll();


        level1.update();


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
