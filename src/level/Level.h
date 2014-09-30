#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <cstdint>
#include <vector>
#include <string>
#include "irrlicht.h"
#include "tinythread.h"
#include "Timer.h"

class Globals;
class TileSet;
class b2World;
class EffectManager;
class Background;
class Element;
class PlayerElement;

class Level
{
public:
    friend class Element;

    Level(Globals*, std::string tileset);
    Level(const Level&) = delete;
    ~Level();

    Globals* getGlobals();
    TileSet* getTileSet();
    b2World* getPhysics();
    EffectManager* getEffectManager();
    void loadLevel(std::string file);
    void reloadLevel();
    void clearLevel();
    Background* getBackground();
    PlayerElement* getPlayerElement();
    unsigned getRewardSum() const;
    void setDimension(irr::core::dimension2du);
    irr::core::dimension2du getDimension() const;
    void setUnitSize(unsigned);
    unsigned getUnitSize() const;
    irr::core::vector2di getViewOffset() const;
    irr::core::rect<irr::s32> getView() const;
    irr::core::vector2di getScreenPosition(Element*) const;
    irr::core::vector2di getScreenPosition(irr::core::vector2df) const;
    void update();

protected:
    void addElement(Element*);
    void removeElement(Element*);
    PlayerElement* findPlayer();
    void processInsertionQueue();
    void processDeletionQueue();
    void updateView(uint32_t elapsedMs);
    bool isElementOnScreen(Element*);

private:
    mutable tthread::recursive_mutex m_mutex;
    Globals* m_globals;
    TileSet* m_tileset;
    b2World* m_physics;
    EffectManager* m_effectMgr;
    std::string m_levelFile;
    irr::core::vector2di m_offset;
    irr::core::dimension2du m_dimension;
    unsigned m_unit;
    Background* m_bg;
    std::vector<Element*> m_elements;
    std::vector<Element*> m_elemInsertionQueue;
    std::vector<Element*> m_elemDeletionQueue;
    Timer m_timer;
    PlayerElement* m_player;
    unsigned m_rewardSum;
    irr::core::vector2di m_camMovement;
};

#endif // LEVEL_H_INCLUDED
