#include <dirent.h>
#include <fstream>
#include "Box2D\Box2D.h"
#include "Globals.h"
#include "Parser.h"
#include "Level.h"
#include "TileSet.h"
#include "elements\Element.h"
#include "motions\Motion.h"

using namespace irr;

static b2BodyType motionTypeToBodyType(Motion::Type motionType)
{
    switch (motionType)
    {
        case Motion::Type::DYNAMIC:
        case Motion::Type::AUTO:
            return b2_dynamicBody;

        case Motion::Type::STRAIGHT:
        case Motion::Type::CIRCULAR:
            return b2_kinematicBody;

        case Motion::Type::STATIC:
        default:
            return b2_staticBody;
    }
}

Shape TileData::getBoundingShape(core::vector2di imgPosition) const
{
    unsigned tileId = (tileDimension.X * imgPosition.Y) + imgPosition.X;
    auto it = boundings.find(tileId);
    if (it == boundings.end())
        return {};
    else
        return it->second;
}

b2Body* TileData::createBody(Element* element) const
{
    b2Body* body = nullptr;
    Motion::Type motionType = element->getMotionType();
    Element::Type elementType = element->getType();

    if (motionType != Motion::Type::NONE ||
        elementType == Element::Type::PARTICLE)
    {
        Shape shape = element->getTileData()->getBoundingShape(element->getImagePosition());
        core::vector2df position = element->getPosition();
        float scale = element->getScale();

        b2BodyDef bodyDef;
        bodyDef.type = motionTypeToBodyType(motionType);
        if (elementType == Element::Type::PARTICLE) bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(position.X, position.Y);
        bodyDef.fixedRotation = true; // do not rotate!
        bodyDef.userData = element;

        body = element->getLevel()->getPhysics()->CreateBody(&bodyDef);
        shape.addToBody(body, scale);
    }

    return body;
}

core::vector2di TileData::getImagePosition(unsigned imgNum) const
{
    core::vector2di imgPos(0, 0);
    imgPos.Y = imgNum / tileDimension.X;
    imgPos.X = imgNum % tileDimension.X;
    return imgPos;
}

TileData::Animation* TileData::getAnimation(irr::core::vector2di imgPos)
{
    auto it = animations.find(imgPos);
    if (it != animations.end())
        return &it->second;
    else
        return nullptr;
}

const TileData::Animation* TileData::getAnimation(irr::core::vector2di imgPos) const
{
    auto it = animations.find(imgPos);
    if (it != animations.end())
        return &it->second;
    else
        return nullptr;
}

void TileData::drawTile(Level* level, core::vector2di imgPos, core::vector2df pos, float scale, irr::video::SColor color) const
{
    video::IVideoDriver* driver = level->getGlobals()->driver;
    core::recti screen({0,0}, driver->getScreenSize());

    core::rect<s32> srcRect =
        {(s32)(imgPos.X * tileSize), (s32)(imgPos.Y * tileSize),
        (s32)((imgPos.X + 1) * tileSize), (s32)((imgPos.Y + 1) * tileSize)};

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    video::SColor colors[4] = {color, color, color, color};
    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    if (screen.isRectCollided(destRect))
    {
        driver->draw2DImage(texture, destRect, srcRect, 0, colors, true);
    }
}

void TileData::drawAnimation(AnimationType animType, unsigned animSpeed, Level* level, core::vector2di imgPos,
                             core::vector2df pos, float scale, bool standby, irr::video::SColor color) const
{
    auto it = animations.find(imgPos);
    if (it == animations.end())
    {
        drawTile(level, imgPos, pos, scale, color);
        return;
    }

    const Animation* anim = &(it->second);
    unsigned frame;

    if (standby)
    {
        frame = 0;
    }
    else if (anim->animCount == 1)
    {
        uint32_t elapsedMs = level->getTileSet()->getAnimationTimer()->peekElapsed();
        frame = ((elapsedMs / (1000 / animSpeed)) % (anim->frameCount));
    }
    else
    {
        uint32_t elapsedMs = level->getTileSet()->getAnimationTimer()->peekElapsed();
        frame = ((elapsedMs / (1000 / animSpeed)) % (anim->frameCount - 1)) + 1;
    }

    core::rect<s32> srcRect =
        {(s32)(frame * tileSize), (s32)((unsigned)animType * tileSize),
         (s32)((frame + 1) * tileSize), (s32)(((unsigned)animType + 1) * tileSize)};

    video::IVideoDriver* driver = level->getGlobals()->driver;
    core::recti screen({0,0}, driver->getScreenSize());

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    video::SColor colors[4] = {color, color, color, color};
    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    if (screen.isRectCollided(destRect))
    {
        driver->draw2DImage(anim->texture, destRect, srcRect, 0, colors, true);
    }
}

void TileData::drawContinuousAnimation(unsigned startPoint, unsigned animSpeed, Level* level, core::vector2di imgPos,
                                       core::vector2df pos, float scale, irr::video::SColor color) const
{
    auto it = animations.find(imgPos);
    if (it == animations.end())
    {
        drawTile(level, imgPos, pos, scale, color);
        return;
    }

    const Animation* anim = &(it->second);
    unsigned tileCount = anim->frameCount * anim->animCount;
    uint32_t elapsedMs = level->getTileSet()->getAnimationTimer()->peekElapsed();
    unsigned frame = (elapsedMs / (1000 / animSpeed) + startPoint) % tileCount;

    core::vector2di tile(frame % anim->frameCount, frame / anim->frameCount);
    core::rect<s32> srcRect =
        {(s32)(tile.X * tileSize), (s32)(tile.Y * tileSize),
         (s32)((tile.X + 1) * tileSize), (s32)((tile.Y + 1) * tileSize)};

    video::IVideoDriver* driver = level->getGlobals()->driver;
    core::recti screen({0,0}, driver->getScreenSize());

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    video::SColor colors[4] = {color, color, color, color};
    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    if (screen.isRectCollided(destRect))
    {
        driver->draw2DImage(anim->texture, destRect, srcRect, 0, colors, true);
    }
}


TileSet::TileSet(Globals* globals, std::string name)
 : m_globals(globals)
 , m_name(name)
{
    std::string basedir("../media/tilesets/");
    basedir += name;
    basedir += "/";

    findBackgroundData(basedir + "background/", m_backgrounds);
    findTileData(basedir + "ground/", m_grounds);
    findTileData(basedir + "enemy/", m_enemies);
    findTileData(basedir + "reward/", m_rewards);
    findTileData(basedir + "player/", m_players);
    findTileData(basedir + "finish/", m_finishes);
    findTileData(basedir + "particle/", m_particles);
    findAnimationData(basedir + "animations/");
}

TileSet::~TileSet()
{
}

std::string TileSet::getName() const
{
    return m_name;
}

const Timer* TileSet::getAnimationTimer() const
{
    return &m_animTimer;
}

irr::video::ITexture* TileSet::getBackground(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_backgrounds.find(id);
        if (it != m_backgrounds.end())
        {
            BackgroundData* data = &(it->second);
            return data->texture;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_backgrounds)
        {
            if (it.first >= id) return it.second.texture;
        }

        return nullptr;
    }
}

const TileData* TileSet::getGroundData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_grounds.find(id);
        if (it != m_grounds.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_grounds)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getEnemyData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_enemies.find(id);
        if (it != m_enemies.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_enemies)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getRewardData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_rewards.find(id);
        if (it != m_rewards.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_rewards)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getPlayerData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_players.find(id);
        if (it != m_players.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_players)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getFinishData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_finishes.find(id);
        if (it != m_finishes.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_finishes)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getParticleData(unsigned id, SearchType search) const
{
    if (search == SearchType::EXACT)
    {
        auto it = m_particles.find(id);
        if (it != m_particles.end())
        {
            TileData* data = &(it->second);
            return data;
        }
        else
            return nullptr;
    }
    else
    {
        if (search == SearchType::NEXT) ++id;

        for (auto& it : m_particles)
        {
            if (it.first >= id) return &(it.second);
        }

        return nullptr;
    }
}

const TileData* TileSet::getData(Element::Type type, unsigned id, SearchType search) const
{
    switch (type)
    {
        case Element::Type::GROUND:
            return getGroundData(id, search);

        case Element::Type::ENEMY:
            return getEnemyData(id, search);

        case Element::Type::REWARD:
            return getRewardData(id, search);

        case Element::Type::PLAYER:
            return getPlayerData(id, search);

        case Element::Type::FINISH:
            return getFinishData(id, search);

        default:
            return nullptr;
    }
}

bool TileSet::fillTileData(std::string dirName, std::string fileName, unsigned& id, TileData& data) const
{
    if (fileName.rfind(".png") == std::string::npos) return false; // not png

    try
    {
        data.fileName = dirName + fileName;
        data.texture = m_globals->driver->getTexture(data.fileName.c_str());

        Parser tileParser(fileName, '_');
        std::tie(id, data.tileSize, data.tileDimension.X, data.tileDimension.Y, data.tileCount) =
            tileParser.getArgs<unsigned, unsigned, int, int, unsigned>();

        std::string txtname = dirName + fileName;
        txtname.replace(txtname.end()-3, txtname.end(), "txt");

        std::fstream f(txtname);
        Parser bbParser(f, ',');

        try
        {
            // set up special bounding boxes
            do {
                unsigned id = bbParser.getArg<unsigned>();
                data.boundings[id] = bbParser.getArg<Shape>();
            }
            while (bbParser.nextLine());
        }
        catch (...) {}

        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "TileData error: " << fileName << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

void TileSet::findTileData(std::string dirName, std::map<unsigned, TileData>& data) const
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            unsigned id;
            TileData td;
            if (fillTileData(dirName, ent->d_name, id, td))
            {
                TileData& savedtd = data[id];
                savedtd = td;
                for (auto& it : td.boundings) savedtd.boundings.insert(it);
            }
        }
        closedir(dir);
    }
}

void TileSet::findAnimationData(std::string dirName) const
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            std::string baseName = ent->d_name;
            std::string fileName = dirName + baseName;
            std::map<unsigned, TileData>* elemTypeTiles = nullptr;

            try
            {
                Element::Type tileType;
                unsigned tileId;
                core::vector2di imgPos;
                Parser p(baseName, '_');

                std::tie(tileType, tileId, imgPos.X, imgPos.Y) = p.getArgs<Element::Type, unsigned, unsigned, unsigned>();

                switch (tileType)
                {
                    case Element::Type::GROUND:
                        elemTypeTiles = &m_grounds;
                        break;

                    case Element::Type::ENEMY:
                        elemTypeTiles = &m_enemies;
                        break;

                    case Element::Type::REWARD:
                        elemTypeTiles = &m_rewards;
                        break;

                    case Element::Type::PLAYER:
                        elemTypeTiles = &m_players;
                        break;

                    case Element::Type::FINISH:
                        elemTypeTiles = &m_finishes;
                        break;

                    case Element::Type::PARTICLE:
                        elemTypeTiles = &m_particles;
                        break;

                    default:
                        throw std::runtime_error("unknown element type");
                }

                auto tileIter = elemTypeTiles->find(tileId);
                if (tileIter != elemTypeTiles->end())
                {
                    irr::video::ITexture* animTexture = m_globals->driver->getTexture(fileName.c_str());

                    if (animTexture)
                    {
                        TileData* td = &tileIter->second;
                        TileData::Animation* anim = &td->animations[imgPos]; // this will do the insertion to the std::map also

                        core::dimension2du animTextureSize = animTexture->getSize();
                        anim->texture = animTexture;
                        anim->animCount = animTextureSize.Height / td->tileSize;
                        anim->frameCount = animTextureSize.Width / td->tileSize;

                        //std::cout << "Animation loaded: " << baseName << "!" << std::endl;
                        //std::cout << "animCount: " << anim->animCount << ", frameCount: " << anim->frameCount << std::endl;
                    }
                }
            }
            catch (const std::exception& e)
            {
                std::cout << "Animation error: " << fileName << " (" << e.what() << ")" << std::endl;
            }
        }

        closedir(dir);
    }
}

bool TileSet::fillBackgroundData(std::string dirName, std::string fileName, unsigned& id, BackgroundData& data) const
{
    try
    {
        Parser p(fileName, '_');

        data.fileName = dirName + fileName;
        data.texture = m_globals->driver->getTexture(data.fileName.c_str());
        id = p.getArg<unsigned>();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "BackgroundData error: " << fileName << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

void TileSet::findBackgroundData(std::string dirName, std::map<unsigned, BackgroundData>& data) const
{
    DIR *dir;
    struct dirent *ent;
    unsigned id;
    BackgroundData bd;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            if (fillBackgroundData(dirName, ent->d_name, id, bd))
            {
                data[id] = bd;
            }
        }
        closedir(dir);
    }
}
