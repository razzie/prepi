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
    enum SearchType
    {
        EXACT,
        BEST,
        NEXT
    };

    TileSet(Globals*, std::string name);
    ~TileSet();

    std::string getName() const;
    irr::video::ITexture* getBackground(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getGroundData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getEnemyData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getRewardData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getPlayerData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getFinishData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getData(Element::Type type, unsigned id, SearchType = SearchType::EXACT) const;

private:
    Globals* m_globals;
    std::string m_name;
    mutable std::map<unsigned, BackgroundData> m_backgrounds;
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
