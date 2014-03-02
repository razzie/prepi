#include <dirent.h>
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
    findTileSetData(basedir + "ground/", m_grounds);
    findTileSetData(basedir + "enemy/", m_enemies);
    findTileSetData(basedir + "reward/", m_rewards);
    findTileSetData(basedir + "player/", m_players);
    findTileSetData(basedir + "finish/", m_finishes);
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
        const BackgroundData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data->texture;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getGroundData(unsigned id) const
{
    auto it = m_grounds.find(id);
    if (it != m_grounds.end())
    {
        const TileSetData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getEnemyData(unsigned id) const
{
    auto it = m_enemies.find(id);
    if (it != m_enemies.end())
    {
        const TileSetData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getRewardData(unsigned id) const
{
    auto it = m_rewards.find(id);
    if (it != m_rewards.end())
    {
        const TileSetData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getPlayerData(unsigned id) const
{
    auto it = m_players.find(id);
    if (it != m_players.end())
    {
        const TileSetData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getFinishData(unsigned id) const
{
    auto it = m_finishes.find(id);
    if (it != m_finishes.end())
    {
        const TileSetData* data = &(it->second);
        if (data->texture == nullptr)
            data->texture = m_globals->driver->getTexture(data->fileName.c_str());
        return data;
    }
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getData(Element::Type type, unsigned id) const
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

bool TileSet::fillTileSetData(std::string dirName, std::string fileName, unsigned& id, TileSetData& data) const
{
    try
    {
        Parser p(fileName, '_');

        data.fileName = dirName + fileName;
        data.texture = nullptr;
        std::tie(id, data.tileSize, data.tileDimension.X, data.tileDimension.Y, data.tileCount) =
            p.getArgs<unsigned, unsigned, int, int, unsigned>();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "TileSetData error: " << fileName << " (" << e.what() << ")" << std::endl;
        return false;
    }
}

void TileSet::findTileSetData(std::string dirName, std::map<unsigned, TileSetData>& data) const
{
    DIR *dir;
    struct dirent *ent;
    unsigned id;
    TileSetData tsd;

    if ((dir = opendir( dirName.c_str() )) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            if (fillTileSetData(dirName, ent->d_name, id, tsd))
            {
                data[id] = tsd;
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
