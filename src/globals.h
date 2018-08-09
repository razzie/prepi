/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "irrlicht.h"

class EventListener;
namespace gg { class application; }

class Globals
{
public:
    irr::IrrlichtDevice* const device;
    irr::video::IVideoDriver* const driver;
    irr::scene::ISceneManager* const smgr;
    irr::gui::IGUIEnvironment* const guienv;
    irr::gui::IGUIFont* const font;
    EventListener* const eventListener;
    gg::application* const app;

    Globals();
    Globals(const Globals&) = delete;
    ~Globals();

    void drawImage(const irr::video::ITexture* texture, irr::core::rect<irr::s32> sourceRect, irr::core::rect<irr::s32> destRect,
                   irr::f32 rotation = 0.f, irr::video::SColor color = {255, 255, 255, 255});
};

#endif // GLOBALS_H_INCLUDED
