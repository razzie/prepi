#include <iostream>
#include <sstream>
#include "Box2D\Box2D.h"
#include "SFML\Audio.hpp"
#include "gglib.hpp"
#include "Globals.h"
#include "Element.h"
#include "PlayerElement.h"
#include "Background.h"
#include "Level.h"
#include "LevelGenerator.h"
#include "GUI.h"
#include "Parser.h"

using namespace std;
using namespace irr;
using namespace sf;

int main()
{
    Globals g;
    bool quit = false;

    /*SoundBuffer rofiBuf;
    rofiBuf.loadFromFile("../media/rofoges.wav");
    Sound rofi(rofiBuf);
    rofi.play();*/

    Level level1(&g, "tale");
    level1.loadLevel("../levels/new_shit.txt");
    GUI gui(&g, &level1);

    gg::script_engine* se = g.app->get_script_engine();

    se->add_function("loadLevel", [&](std::string level)
                     {
                         std::string file = "../levels/" + level + ".txt";
                         level1.loadLevel(file);
                     });

    se->add_function("randomLevel", [&](unsigned columns, unsigned rows)
                     {
                         LevelGenerator lg;
                         lg.generate(&level1, columns, rows);
                     });

    se->add_function("setBackground", [&](unsigned id, unsigned mode)
                     {
                         level1.getBackground()->setId(id);
                         level1.getBackground()->setDrawingMethod(static_cast<Background::DrawingMethod>(mode));
                     });

    se->add_function("setDimension", [&](unsigned columns, unsigned rows)
                     { level1.setDimension({columns, rows}); });

    se->add_function("setUnitSize", [&](unsigned unit)
                     { level1.setUnitSize(unit); });

    se->add_function("movePlayer", [&](float x, float y)
                     {
                         Element* player = level1.getPlayerElement();
                         if (player) player->setPosition({x,y});
                     });

    se->add_function("color", [](unsigned R, unsigned G, unsigned B)
                     {
                         gg::console::output* o = gg::console::get_invoker_output();
                         o->set_color( {(uint8_t)R, (uint8_t)G, (uint8_t)B} );
                         std::cout << "(R: " << R << ", G: " << G << ", B: " << B << ")";
                     });

    se->add_function("setSpeed", [&](float speed)
                     {
                         PlayerElement* player = level1.getPlayerElement();
                         if (player) player->setSpeed(speed);
                     });

    se->add_function("setClimbingMode", [&](irr::f32 climbTreshold)
                     {
                        if (climbTreshold>=1.f || climbTreshold<=-1.0f)
                        {
                            std::cout << "Szarjál sünt!";
                            return;
                        }
                        PlayerElement* player = level1.getPlayerElement();
                        if (player) player->setClimbingMode(climbTreshold);
                     });

    se->add_function("exit", [&]{ quit = true; });

    gg::console* con = g.app->create_console();
    con->open();
    con->drop();

    u32 frames=0;

    while(g.device->run() && !quit)
    {
        g.driver->beginScene(true, true, video::SColor(0,100,100,100));

        //g.smgr->drawAll();

        //tthread::this_thread::sleep_for(tthread::chrono::milliseconds(40));

        level1.update();
        gui.update();

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
