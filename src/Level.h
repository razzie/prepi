#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <list>
#include <string>
#include "irrlicht.h"
#include "tinythread.h"
#include "Timer.h"

class Globals;
class TileSet;
class b2World;
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
    void loadLevel(std::string file);
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
    void update();

protected:
    void addElement(Element*);
    void removeElement(Element*);
    PlayerElement* findPlayer();
    void processInsertionQueue();
    void processDeletionQueue();
    void updateView();
    bool isElementOnScreen(Element*);

private:
    mutable tthread::recursive_mutex m_mutex;
    Globals* m_globals;
    TileSet* m_tileset;
    b2World* m_physics;
    irr::core::vector2di m_offset;
    irr::core::dimension2du m_dimension;
    unsigned m_unit;
    Background* m_bg;
    std::list<Element*> m_elements;
    std::list<Element*> m_elemInsertionQueue;
    std::list<Element*> m_elemDeletionQueue;
    PlayerElement* m_player;
    unsigned m_rewardSum;
    Timer m_timer;
};

#endif // LEVEL_H_INCLUDED
