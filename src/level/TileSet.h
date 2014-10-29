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
class TileSet;

struct BackgroundData
{
    std::string fileName;
    irr::video::ITexture* texture;
};

class TileData
{
public:
    class Animation
    {
    public:
        enum Type
        {
            IDLE_RIGHT = 0,
            IDLE_LEFT = 1,
            RIGHT = 2,
            LEFT = 3,
            JUMP_RIGHT = 4,
            JUMP_LEFT = 5,
            LADDER_UP = 6,
            LADDER_DOWN = 7,
            CLIMB_UP_RIGHT = 8,
            CLIMB_DOWN_RIGHT = 9,
            CLIMB_UP_LEFT = 10,
            CLIMB_DOWN_LEFT = 11,
            APPEAR = 12,
            DISAPPEAR = 13
        };

        Animation();
        ~Animation();

        irr::video::ITexture* m_texture;
        unsigned m_speed;
        unsigned m_frames;
        unsigned m_framesPerRow;
        unsigned m_rows;
    };

    TileData();
    ~TileData();

    const irr::video::ITexture* getTexture() const;
    unsigned getTileSize() const;
    Shape getBoundingShape(irr::core::vector2di imgPosition) const;
    b2Body* createBody(Element*) const;
    irr::core::vector2di getImagePosition(unsigned imgNum) const;
    Animation* getAnimation(irr::core::vector2di imgPos, unsigned animType);
    const Animation* getAnimation(irr::core::vector2di imgPos, unsigned animType) const;

    void drawTile(Level* level, irr::core::vector2di imgPos, irr::core::vector2df pos,
                  float scale = 1.0f, float rotation = 0.f, irr::video::SColor = {255, 255, 255, 255}) const;
    void drawAnimation(Level* level, irr::core::vector2di imgPos, unsigned animType, float speed, irr::core::vector2df pos,
                       float scale = 1.0f, float rotation = 0.f, irr::video::SColor = {255, 255, 255, 255},
                       unsigned startingFrame = 0) const;

private:
    friend class TileSet;

    std::string m_fileName;
    irr::video::ITexture* m_texture;
    unsigned m_tileSize;
    irr::core::vector2di m_tileDimension;
    unsigned m_tileCount;
    std::map<unsigned, Shape> m_boundings;
    std::map<irr::core::vector2di, std::map<unsigned, Animation>> m_animations;
};

class TileSet
{
public:
    TileSet(Globals*, std::string name);
    ~TileSet();

    std::string getName() const;
    const Timer* getAnimationTimer() const;
    irr::video::ITexture* getBackground(unsigned id) const;
    const TileData* getGroundData(unsigned id) const;
    const TileData* getEnemyData(unsigned id) const;
    const TileData* getRewardData(unsigned id) const;
    const TileData* getPlayerData(unsigned id) const;
    const TileData* getFinishData(unsigned id) const;
    const TileData* getParticleData(unsigned id) const;
    const TileData* getData(Element::Type type, unsigned id) const;
    std::map<unsigned, BackgroundData>::const_iterator getBackgroundIterator() const;
    std::map<unsigned, TileData>::const_iterator getTileIterator(Element::Type type) const;
    void reload();

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
