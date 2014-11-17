#include <ctime>
#include "Globals.h"
#include "EventListener.h"
#include "gg/application.hpp"

using namespace std;
using namespace irr;
//using namespace sf;

static IrrlichtDevice* createIrrlichtDevice()
{
    SIrrlichtCreationParameters params; // constructor sets the defaults
    params.AntiAlias = 1;//16;
    params.Bits = 32;
    params.DriverType = video::EDT_OPENGL;
    params.Fullscreen = false;
    params.WindowSize = core::dimension2d<u32>(1366, 768);
    params.WithAlphaChannel = true;
    params.Vsync = true;

    return createDeviceEx(params);
}

Globals::Globals()
 : device(createIrrlichtDevice())
 , driver(device->getVideoDriver())
 , smgr(device->getSceneManager())
 , guienv(device->getGUIEnvironment())
 , font(guienv->getFont("../media/font.png"))
 , eventListener(new EventListener())
 , app(gg::application::create("prepi"))
{
    srand(time(NULL));

    device->setEventReceiver(eventListener);
    device->setResizable(true);
    device->setWindowCaption(L"prepi");
    smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));

    driver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, true);
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
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

void Globals::drawImage(const video::ITexture* texture, core::rect<s32> sourceRect, core::rect<s32> destRect,
                        f32 rotation, video::SColor color)
{
    video::SMaterial material;// = driver->getMaterial2D();

    // Store and clear the projection matrix
    core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());

    // Store and clear the view matrix
    core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
    driver->setTransform(video::ETS_VIEW, core::matrix4());

    // Find the positions of corners
    core::vector2di corner[4];
    corner[0] = core::vector2di(destRect.UpperLeftCorner.X, destRect.UpperLeftCorner.Y);
    corner[1] = core::vector2di(destRect.LowerRightCorner.X, destRect.UpperLeftCorner.Y);
    corner[2] = core::vector2di(destRect.UpperLeftCorner.X, destRect.LowerRightCorner.Y);
    corner[3] = core::vector2di(destRect.LowerRightCorner.X, destRect.LowerRightCorner.Y);;

    // Rotate corners
    if (rotation != 0.0f)
    {
        for (int x = 0; x < 4; x++)
            corner[x].rotateBy(rotation, destRect.getCenter());
    }

    // Find the uv coordinates of the sourceRect
    core::vector2df invTexSize(1.f / texture->getSize().Width, 1.f  / texture->getSize().Height);
    core::vector2df uvCorner[4];
    uvCorner[0] = core::vector2df(sourceRect.UpperLeftCorner.X, sourceRect.UpperLeftCorner.Y) * invTexSize;
    uvCorner[1] = core::vector2df(sourceRect.LowerRightCorner.X, sourceRect.UpperLeftCorner.Y) * invTexSize;
    uvCorner[2] = core::vector2df(sourceRect.UpperLeftCorner.X, sourceRect.LowerRightCorner.Y) * invTexSize;
    uvCorner[3] = core::vector2df(sourceRect.LowerRightCorner.X, sourceRect.LowerRightCorner.Y) * invTexSize;

    // Vertices for the image
    video::S3DVertex vertices[4];
    u16 indices[6] = { 0, 1, 2, 3 ,2 ,1 };

    // Convert pixels to world coordinates
    float screenWidth = driver->getScreenSize().Width;
    float screenHeight = driver->getScreenSize().Height;
    for (int x = 0; x < 4; x++)
    {
        float screenPosX = ((corner[x].X / screenWidth) - 0.5f) * 2.0f;
        float screenPosY = ((corner[x].Y / screenHeight) - 0.5f) * -2.0f;
        vertices[x].Pos = core::vector3df(screenPosX, screenPosY, 1);
        vertices[x].TCoords = uvCorner[x];
        vertices[x].Color = color;
    }
    material.Lighting = false;
    material.ZWriteEnable = false;
    material.ZBuffer = false;
    material.TextureLayer[0].Texture = const_cast<video::ITexture*>(texture);
    material.MaterialType = video::EMT_ONETEXTURE_BLEND; // video::EMT_TRANSPARENT_ALPHA_CHANNEL; // video::EMT_SOLID;
    material.MaterialTypeParam =
        video::pack_textureBlendFunc(irr::video::EBF_SRC_ALPHA, irr::video::EBF_ONE_MINUS_SRC_ALPHA,
                                     irr::video::EMFN_MODULATE_1X, irr::video::EAS_TEXTURE | irr::video::EAS_VERTEX_COLOR);
    //material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
    //material.TextureLayer[0].TextureWrapV = video::ETC_CLAMP;

    driver->setMaterial(material);
    driver->drawIndexedTriangleList(&vertices[0], 4, &indices[0], 2);

    // Restore projection and view matrices
    driver->setTransform(video::ETS_PROJECTION, oldProjMat);
    driver->setTransform(video::ETS_VIEW, oldViewMat);
}
