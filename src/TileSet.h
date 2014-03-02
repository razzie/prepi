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

    struct BackgroundData
    {
        std::string fileName;
        mutable irr::video::ITexture* texture;
    };

    struct TileSetData
    {
        std::string fileName;
        mutable irr::video::ITexture* texture;
        unsigned tileSize;
        irr::core::vector2di tileDimension;
        unsigned tileCount;
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
    std::map<unsigned, BackgroundData> m_backgrouns;
    std::map<unsigned, TileSetData> m_grounds;
    std::map<unsigned, TileSetData> m_enemies;
    std::map<unsigned, TileSetData> m_rewards;
    std::map<unsigned, TileSetData> m_players;
    std::map<unsigned, TileSetData> m_finishes;

    bool fillTileSetData(std::string dirName, std::string fileName, unsigned& id, TileSetData&) const;
    void findTileSetData(std::string dirName, std::map<unsigned, TileSetData>&) const;
    bool fillBackgroundData(std::string dirName, std::string fileName, unsigned& id, BackgroundData&) const;
    void findBackgroundData(std::string dirName, std::map<unsigned, BackgroundData>&) const;
};

#endif // TILESET_H_INCLUDED
