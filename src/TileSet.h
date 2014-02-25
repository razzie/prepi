#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <string>
#include <map>
#include "irrlicht.h"
#include "Element.h"

class Globals;

class TileSet
{
public:
    TileSet(Globals*, std::string name);
    ~TileSet();

    struct TileSetData
    {
        unsigned unitPixelCount;
        irr::core::position2di position;
        irr::video::ITexture* picture;
    };

    std::string getName() const;
    irr::video::ITexture* getBackground(unsigned id) const;
    const TileSetData* getGroundData(unsigned id) const;
    const TileSetData* getEnemyData(unsigned id) const;
    const TileSetData* getRewardData(unsigned id) const;
    const TileSetData* getPlayerData(unsigned id) const;
    const TileSetData* getFinishData(unsigned id) const;
    const TileSetData* getData(Element::Type type, unsigned id) const;

private:
    Globals* m_globals;
    std::string m_name;
    std::map<unsigned, irr::video::ITexture*> m_backgrouns;
    std::map<unsigned, TileSetData> m_grounds;
    std::map<unsigned, TileSetData> m_enemies;
    std::map<unsigned, TileSetData> m_rewards;
    std::map<unsigned, TileSetData> m_players;
    std::map<unsigned, TileSetData> m_finishes;
};

#endif // TILESET_H_INCLUDED
