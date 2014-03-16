#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <string>
#include <map>
#include "irrlicht.h"
#include "Element.h"

class Globals;

struct BackgroundData
{
    std::string fileName;
    irr::video::ITexture* texture;
};

struct TileData
{
    std::string fileName;
    irr::video::ITexture* texture;
    unsigned tileSize;
    irr::core::vector2di tileDimension;
    unsigned tileCount;
    std::map<unsigned, irr::core::rectf> boundingBoxes;

    irr::core::rectf getBoundingBox(irr::core::vector2di imgPosition) const;
};

class TileSet
{
public:
    TileSet(Globals*, std::string name);
    ~TileSet();

    std::string getName() const;
    irr::video::ITexture* getBackground(unsigned id) const;
    const TileData* getGroundData(unsigned id) const;
    const TileData* getEnemyData(unsigned id) const;
    const TileData* getRewardData(unsigned id) const;
    const TileData* getPlayerData(unsigned id) const;
    const TileData* getFinishData(unsigned id) const;
    const TileData* getData(Element::Type type, unsigned id) const;

private:
    Globals* m_globals;
    std::string m_name;
    mutable std::map<unsigned, BackgroundData> m_backgrouns;
    mutable std::map<unsigned, TileData> m_grounds;
    mutable std::map<unsigned, TileData> m_enemies;
    mutable std::map<unsigned, TileData> m_rewards;
    mutable std::map<unsigned, TileData> m_players;
    mutable std::map<unsigned, TileData> m_finishes;

    bool fillTileData(std::string dirName, std::string fileName, unsigned& id, TileData&) const;
    void findTileData(std::string dirName, std::map<unsigned, TileData>&) const;
    bool fillBackgroundData(std::string dirName, std::string fileName, unsigned& id, BackgroundData&) const;
    void findBackgroundData(std::string dirName, std::map<unsigned, BackgroundData>&) const;
};

#endif // TILESET_H_INCLUDED
