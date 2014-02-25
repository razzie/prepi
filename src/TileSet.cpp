#include "Globals.h"
#include "TileSet.h"

using namespace irr;

static std::map<unsigned, io::path> bgs = {
    {1, "../media/tilesets/tale/background/01_Clear sky.png"},
    {2, "../media/tilesets/tale/background/02_Dark.png"},
    {3, "../media/tilesets/tale/background/03_Fantasy.jpg"},
    {4, "../media/tilesets/tale/background/04_Magic.jpg"}};

TileSet::TileSet(Globals* globals, std::string name)
 : m_globals(globals)
 , m_name(name)
{

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
    /*auto it = m_backgrouns.find(id);
    if (it != m_backgrouns.end())
        return it->second;
    else
        return nullptr;*/

    return m_globals->driver->getTexture(bgs[id]);
}

const TileSet::TileSetData* TileSet::getGroundData(unsigned id) const
{
    auto it = m_grounds.find(id);
    if (it != m_grounds.end())
        return &(it->second);
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getEnemyData(unsigned id) const
{
    auto it = m_enemies.find(id);
    if (it != m_enemies.end())
        return &(it->second);
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getRewardData(unsigned id) const
{
    auto it = m_rewards.find(id);
    if (it != m_rewards.end())
        return &(it->second);
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getPlayerData(unsigned id) const
{
    auto it = m_players.find(id);
    if (it != m_players.end())
        return &(it->second);
    else
        return nullptr;
}

const TileSet::TileSetData* TileSet::getFinishData(unsigned id) const
{
    auto it = m_finishes.find(id);
    if (it != m_finishes.end())
        return &(it->second);
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
