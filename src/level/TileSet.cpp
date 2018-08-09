/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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

static b2Filter elementTypeToFilter(Element::Type elemType)
{
    b2Filter filter;

    switch (elemType)
    {
        case Element::Type::GROUND:
        case Element::Type::ENEMY:
        case Element::Type::REWARD:
        case Element::Type::PLAYER:
        //case Element::Type::FINISH:
            filter.categoryBits = 0x0001;
            filter.maskBits = 0x0003;
            break;

        case Element::Type::PARTICLE:
            filter.categoryBits = 0x0002;
            filter.maskBits = 0x0003;
            break;

        // type(s) below will never collide with blood
        case Element::Type::FINISH:
            filter.categoryBits = 0x0001;
            filter.maskBits = 0x0001;
            break;
    }

    return filter;
}

TileData::Animation::Animation()
 : m_tileData(nullptr)
 , m_texture(nullptr)
 , m_speed(0)
 , m_frames(0)
 , m_framesPerRow(0)
 , m_rows(0)
 , m_enabled(false)
{
}

TileData::Animation::Animation(Animation&& anim)
{
    *this = std::move(anim);
}

TileData::Animation::~Animation()
{
    if (m_tileData != nullptr)
    {
        m_tileData->m_tileSet->m_globals->driver->removeTexture(m_texture);
        m_texture = nullptr;
    }
}

TileData::Animation& TileData::Animation::operator= (Animation&& anim)
{
    m_tileData = anim.m_tileData;
    std::swap(m_texture, anim.m_texture);
    m_speed = anim.m_speed;
    m_frames = anim.m_frames;
    m_framesPerRow = anim.m_framesPerRow;
    m_rows = anim.m_rows;
    m_enabled = anim.m_enabled;
    return *this;
}

TileData::TileData()
 : m_tileSet(nullptr)
 , m_texture(nullptr)
 , m_tileSize(0)
 , m_tileDimension(0, 0)
 , m_tileCount(0)
{
}

TileData::TileData(TileData&& td)
{
    *this = std::move(td);
}

TileData::~TileData()
{
    if (m_tileSet != nullptr)
    {
        m_tileSet->m_globals->driver->removeTexture(m_texture);
        m_texture = nullptr;
    }
}

TileData& TileData::operator= (TileData&& td)
{
    m_tileSet = td.m_tileSet;
    std::swap(m_texture, td.m_texture);
    m_tileSize = td.m_tileSize;
    m_tileDimension = td.m_tileDimension;
    m_tileCount = td.m_tileCount;
    std::swap(m_boundings, td.m_boundings);
    std::swap(m_animations, td.m_animations);
    return *this;
}

const video::ITexture* TileData::getTexture() const
{
    return m_texture;
}

unsigned TileData::getTileSize() const
{
    return m_tileSize;
}

core::vector2di TileData::getImagePosition(unsigned imgNum) const
{
    core::vector2di imgPos(0, 0);
    imgPos.Y = imgNum / m_tileDimension.X;
    imgPos.X = imgNum % m_tileDimension.X;
    return imgPos;
}

unsigned TileData::getImageNumber(core::vector2di imgPos) const
{
    return (m_tileDimension.X * imgPos.Y) + imgPos.X;
}

Shape TileData::getBoundingShape(core::vector2di imgPos) const
{
    unsigned tileId = getImageNumber(imgPos);

    if (tileId >= m_tileCount)
        throw std::runtime_error("wrong tile number");

    return m_boundings[tileId];
}

const TileData::Animation* TileData::getAnimation(core::vector2di imgPos, unsigned animType) const
{
    unsigned tileId = getImageNumber(imgPos);

    if (tileId >= m_tileCount)
        throw std::runtime_error("wrong tile number");

    if (animType >= Animation::Type::MAX_ANIM_NUMBER)
        throw std::runtime_error("wrong anim type");

    const Animation* anim = &((m_animations[tileId])[animType]);
    return (anim->m_enabled) ? anim : nullptr;
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

        b2Filter filter = elementTypeToFilter(elementType);
        body->GetFixtureList()->SetFilterData(filter);
    }

    return body;
}

void TileData::drawTile(Level* level, core::vector2di imgPos, core::vector2df pos, float scale, float rotation, video::SColor color) const
{
    Globals* g = level->getGlobals();
    core::recti screen({0,0}, g->driver->getScreenSize());

    core::rect<s32> srcRect =
        {(s32)(imgPos.X * m_tileSize), (s32)(imgPos.Y * m_tileSize),
        (s32)((imgPos.X + 1) * m_tileSize), (s32)((imgPos.Y + 1) * m_tileSize)};

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    if (screen.isRectCollided(destRect))
    {
        g->drawImage(m_texture, srcRect, destRect, rotation, color);
    }
}

void TileData::drawAnimation(Level* level, core::vector2di imgPos, unsigned animType, float speed, core::vector2df pos,
                             float scale, float rotation, video::SColor color, unsigned startingFrame, unsigned* currentFrame) const
{
    const Animation* anim = getAnimation(imgPos, animType);
    if (anim == nullptr)
    {
        drawTile(level, imgPos, pos, scale, rotation, color);
        return;
    }

    Globals* g = level->getGlobals();
    core::recti screen({0,0}, g->driver->getScreenSize());

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    core::rect<s32> destRect = {0, 0, (s32)(scale * unit), (s32)(scale * unit)};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    if (!screen.isRectCollided(destRect))
        return;

    int animSpeed = speed * (float)anim->m_speed;
    uint32_t elapsedMs = level->getTileSet()->getAnimationTimer()->peekElapsed();
    unsigned frame = 0;

    if (animSpeed > 0)
        frame = (((elapsedMs * animSpeed) / 1000) + startingFrame) % anim->m_frames;
    else if (animSpeed < 0)
        frame = anim->m_frames - ((((elapsedMs * -animSpeed) / 1000) + startingFrame) % anim->m_frames);
    else
        frame = startingFrame % anim->m_frames;

    if (currentFrame != nullptr) *currentFrame = frame;

    core::vector2di tile(frame % anim->m_framesPerRow, frame / anim->m_framesPerRow);
    core::rect<s32> srcRect =
        {(s32)(tile.X * m_tileSize), (s32)(tile.Y * m_tileSize),
         (s32)((tile.X + 1) * m_tileSize), (s32)((tile.Y + 1) * m_tileSize)};

    g->drawImage(anim->m_texture, srcRect, destRect, rotation, color);
}


TileSet::TileSet(Globals* globals, std::string name)
 : m_globals(globals)
 , m_name(name)
{
    reload();
}

TileSet::~TileSet()
{
}

void TileSet::reload()
{
    std::string basedir("../media/tilesets/");
    basedir += m_name;
    basedir += "/";

    m_grounds.clear();
    m_enemies.clear();
    m_rewards.clear();
    m_players.clear();
    m_finishes.clear();
    m_particles.clear();

    findBackgroundData(basedir + "background/", m_backgrounds);
    findTileData(basedir + "ground/", m_grounds);
    findTileData(basedir + "enemy/", m_enemies);
    findTileData(basedir + "reward/", m_rewards);
    findTileData(basedir + "player/", m_players);
    findTileData(basedir + "finish/", m_finishes);
    findTileData(basedir + "particle/", m_particles);
    findAnimationData(basedir + "animations/");

    m_animTimer.reset();
}

std::string TileSet::getName() const
{
    return m_name;
}

const Timer* TileSet::getAnimationTimer() const
{
    return &m_animTimer;
}

video::ITexture* TileSet::getBackground(unsigned id) const
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

const TileData* TileSet::getGroundData(unsigned id) const
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

const TileData* TileSet::getEnemyData(unsigned id) const
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

const TileData* TileSet::getRewardData(unsigned id) const
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

const TileData* TileSet::getPlayerData(unsigned id) const
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

const TileData* TileSet::getFinishData(unsigned id) const
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

const TileData* TileSet::getParticleData(unsigned id) const
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

const TileData* TileSet::getData(Element::Type type, unsigned id) const
{
    switch (type)
    {
        case Element::Type::GROUND:
            return getGroundData(id);

        case Element::Type::ENEMY:
            return getEnemyData(id);

        case Element::Type::REWARD:
            return getRewardData(id);

        case Element::Type::PLAYER:
            return getPlayerData(id);

        case Element::Type::FINISH:
            return getFinishData(id);

        case Element::Type::PARTICLE:
            return getParticleData(id);

        default:
            return nullptr;
    }
}

std::map<unsigned, BackgroundData>::const_iterator TileSet::getBackgroundIterator() const
{
    return m_backgrounds.begin();
}

std::map<unsigned, TileData>::const_iterator TileSet::getTileIterator(Element::Type type) const
{
    switch (type)
    {
        case Element::Type::GROUND:
            return m_grounds.begin();

        case Element::Type::ENEMY:
            return m_enemies.begin();

        case Element::Type::REWARD:
            return m_rewards.begin();

        case Element::Type::PLAYER:
            return m_players.begin();

        case Element::Type::FINISH:
            return m_finishes.begin();

        case Element::Type::PARTICLE:
            return m_particles.begin();

        default:
            throw std::runtime_error("unknown element type");
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

            std::string fileName = ent->d_name;
            if (fileName.rfind(".png") == std::string::npos) continue; // not png

            try
            {
                unsigned id;
                TileData td;
                td.m_tileSet = this;
                td.m_fileName = dirName + fileName;
                td.m_texture = m_globals->driver->getTexture(td.m_fileName.c_str());

                // parse information from file name
                Parser tileParser(fileName, '_');
                std::tie(id, td.m_tileSize, td.m_tileDimension.X, td.m_tileDimension.Y, td.m_tileCount) =
                    tileParser.getArgs<unsigned, unsigned, int, int, unsigned>();

                // pre-construct boundings and animations to avoid segfaults
                td.m_boundings.resize(td.m_tileCount);
                td.m_animations.resize(td.m_tileCount);
                for (auto& it : td.m_animations) it.resize(TileData::Animation::Type::MAX_ANIM_NUMBER);

                std::string txtname = dirName + fileName;
                txtname.replace(txtname.end()-3, txtname.end(), "txt");

                std::fstream f(txtname);
                Parser bbParser(f, ',');

                try
                {
                    // set up special bounding boxes
                    do {
                        unsigned id = bbParser.getArg<unsigned>();

                        if (id >= td.m_tileCount)
                            throw std::runtime_error("wrong tile number");

                        td.m_boundings[id] = bbParser.getArg<Shape>();
                    }
                    while (bbParser.nextLine());
                }
                catch (const std::exception& e)
                {
                    std::cout << "Bounding shape error: " << txtname << " (" << e.what() << ")" << std::endl;
                }

                data[id] = std::move(td);
            }
            catch (const std::exception& e)
            {
                std::cout << "TileData error: " << fileName << " (" << e.what() << ")" << std::endl;
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
                unsigned animType;
                unsigned speed;
                Parser p(baseName, '_');

                std::tie(tileType, tileId, imgPos.X, imgPos.Y, animType, speed) =
                    p.getArgs<Element::Type, unsigned, unsigned, unsigned, unsigned, unsigned>();

                if (animType >= TileData::Animation::Type::MAX_ANIM_NUMBER)
                    throw std::runtime_error("unknown anim type");

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
                    TileData* td = &tileIter->second;
                    unsigned tileId = td->getImageNumber(imgPos);

                    if (tileId >= td->m_tileCount)
                        throw std::runtime_error("wrong tile number");

                    video::ITexture* animTexture = m_globals->driver->getTexture(fileName.c_str());
                    core::dimension2du animTextureSize = animTexture->getSize();

                    if (animTexture)
                    {
                        if (animTextureSize.Width % td->m_tileSize != 0 ||
                            animTextureSize.Height % td->m_tileSize)
                        {
                            throw std::runtime_error("tile and animation size mismatch");
                        }

                        TileData::Animation* anim = &(td->m_animations[tileId][animType]);
                        anim->m_tileData = td;
                        anim->m_texture = animTexture;
                        anim->m_speed = speed;
                        anim->m_rows = animTextureSize.Height / td->m_tileSize;
                        anim->m_framesPerRow = animTextureSize.Width / td->m_tileSize;
                        anim->m_frames = anim->m_framesPerRow * anim->m_rows;
                        anim->m_enabled = true;
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

            std::string fileName = ent->d_name;

            try
            {
                Parser p(fileName, '_');

                bd.fileName = dirName + fileName;
                bd.texture = m_globals->driver->getTexture(bd.fileName.c_str());
                id = p.getArg<unsigned>();

                data[id] = bd;
            }
            catch (const std::exception& e)
            {
                std::cout << "BackgroundData error: " << fileName << " (" << e.what() << ")" << std::endl;
            }
        }
        closedir(dir);
    }
}
