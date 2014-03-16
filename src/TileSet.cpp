#include <dirent.h>
#include <fstream>
#include "Globals.h"
#include "TileSet.h"
#include "Parser.h"

using namespace irr;

TileSet::TileSet(Globals* globals, std::string name)
 : m_globals(globals)
 , m_name(name)
{
    std::string basedir("../media/tilesets/");
    basedir += name;
    basedir += "/";

    findBackgroundData(basedir + "background/", m_backgrouns);
    findTileData(basedir + "ground/", m_grounds);
    findTileData(basedir + "enemy/", m_enemies);
    findTileData(basedir + "reward/", m_rewards);
    findTileData(basedir + "player/", m_players);
    findTileData(basedir + "finish/", m_finishes);
}

TileSet::~TileSet()
{
}

std::string TileSet::getName() const
{
    return m_name;
}

irr::video::ITexture* TileSet::getBackground(unsigned id) const
{
    auto it = m_backgrouns.find(id);
    if (it != m_backgrouns.end())
    {
        BackgroundData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
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
        data.texture = nullptr;

        Parser tileParser(fileName, '_');
        std::tie(id, data.tileSize, data.tileDimension.X, data.tileDimension.Y, data.tileCount) =
            tileParser.getArgs<unsigned, unsigned, int, int, unsigned>();

        // set up default bounding boxes
        for (unsigned i = 0; i < data.tileCount; ++i)
            data.boundingBoxes.insert( std::make_pair(i, core::rectf(0.f, 0.f, 1.f, 1.f)) );

        std::string txtname = dirName + fileName;
        txtname.replace(txtname.end()-3, txtname.end(), "txt");

        std::fstream f(txtname);
        Parser bbParser(f, ',');

        try
        {
            // set up special bounding boxes
            do {
                unsigned id;
                core::vector2df upperLeft;
                core::vector2df lowerRight;
                std::tie(id, upperLeft, lowerRight) = bbParser.getArgs<unsigned, core::vector2df, core::vector2df>();
                data.boundingBoxes[id] = {upperLeft, lowerRight};
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
                for (auto& it : td.boundingBoxes) savedtd.boundingBoxes.insert(it);
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
        data.texture = nullptr;
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
