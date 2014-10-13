#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <string>
#include <map>
#include "irrlicht.h"
#include "elements\Element.h"
#include "Shape.h"
#include "Timer.h"

class Globals;
class b2Body;

struct BackgroundData
{
    std::string fileName;
    irr::video::ITexture* texture;
};

struct TileData
{
    struct Animation
    {
        enum class Type : unsigned
        {
            RIGHT = 0,
            LEFT,
            JUMP,
            FRONT,
            LEFT_TO_FRONT,
            RIGHT_TO_FRONT,
            BACK,
            RIGHT_TO_BACK,
            LEFT_TO_BACK
        };

        irr::video::ITexture* texture;
        unsigned animCount;
        unsigned frameCount;
    };

    class AnimationType // helps conversion between unsigned and Animation::Type
    {
    private:
        unsigned m_animType;

    public:
        AnimationType(unsigned animType) : m_animType(animType) {}
        AnimationType(Animation::Type animType) : m_animType(static_cast<unsigned>(animType)) {}
        operator unsigned() const { return m_animType; }
        operator Animation::Type() const { return static_cast<Animation::Type>(m_animType); }
    };

    std::string fileName;
    irr::video::ITexture* texture;
    unsigned tileSize;
    irr::core::vector2di tileDimension;
    unsigned tileCount;
    std::map<unsigned, Shape> boundings;
    std::map<irr::core::vector2di, Animation> animations;

    Shape getBoundingShape(irr::core::vector2di imgPosition) const;
    b2Body* createBody(Element*) const;
    irr::core::vector2di getImagePosition(unsigned imgNum) const;
    Animation* getAnimation(irr::core::vector2di imgPos);
    const Animation* getAnimation(irr::core::vector2di imgPos) const;

    void drawTile(Level* level, irr::core::vector2di imgPos, irr::core::vector2df pos,
                  float scale = 1.0f, irr::video::SColor = {255, 255, 255, 255}) const;
    void drawAnimation(AnimationType animType, unsigned animSpeed, Level* level,
                       irr::core::vector2di imgPos, irr::core::vector2df pos,
                       float scale = 1.0f, bool standby = false, irr::video::SColor = {255, 255, 255, 255}) const;
    void drawContinuousAnimation(unsigned startPoint, unsigned animSpeed, Level* level,
                                 irr::core::vector2di imgPos, irr::core::vector2df pos,
                                 float scale = 1.0f, irr::video::SColor = {255, 255, 255, 255}) const;
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
    const Timer* getAnimationTimer() const;
    irr::video::ITexture* getBackground(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getGroundData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getEnemyData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getRewardData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getPlayerData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getFinishData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getParticleData(unsigned id, SearchType = SearchType::EXACT) const;
    const TileData* getData(Element::Type type, unsigned id, SearchType = SearchType::EXACT) const;

private:
    Globals* m_globals;
    std::string m_name;
    Timer m_animTimer;
    mutable std::map<unsigned, BackgroundData> m_backgrounds;
    mutable std::map<unsigned, TileData> m_grounds;
    mutable std::map<unsigned, TileData> m_enemies;
    mutable std::map<unsigned, TileData> m_rewards;
    mutable std::map<unsigned, TileData> m_players;
    mutable std::map<unsigned, TileData> m_finishes;
    mutable std::map<unsigned, TileData> m_particles;

    bool fillTileData(std::string dirName, std::string fileName, unsigned& id, TileData&) const;
    void findTileData(std::string dirName, std::map<unsigned, TileData>&) const;
    //bool fillAnimationData(std::string dirName, std::string fileName, const TileData&, TileData::Animation&) const;
    void findAnimationData(std::string dirName) const;
    bool fillBackgroundData(std::string dirName, std::string fileName, unsigned& id, BackgroundData&) const;
    void findBackgroundData(std::string dirName, std::map<unsigned, BackgroundData>&) const;
};

#endif // TILESET_H_INCLUDED
