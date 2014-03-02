#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <set>
#include <string>
#include "irrlicht.h"
#include "tinythread.h"

class Globals;
class TileSet;
class Background;
class Element;
class PlayerElement;

class Level
{
public:
    Level(Globals*, std::string tileset, std::string url);
    Level(const Level&) = delete;
    ~Level();

    Globals* getGlobals();
    TileSet* getTileSet();
    void addElement(Element*);
    void removeElement(Element*);
    Background* getBackground();
    PlayerElement* getPlayerElement();
    void setDimension(irr::core::dimension2du);
    irr::core::dimension2du getDimension() const;
    void setUnitSize(unsigned);
    unsigned getUnitSize() const;
    irr::core::vector2di getViewOffset() const;
    irr::core::rect<irr::s32> getView() const;
    void update();

private:
    mutable tthread::mutex m_mutex;
    Globals* m_globals;
    TileSet* m_tileset;
    irr::core::vector2di m_offset;
    irr::core::dimension2du m_dimension;
    unsigned m_unit;
    Background* m_bg;
    std::set<Element*> m_elements;
    PlayerElement* m_player;
};

#endif // LEVEL_H_INCLUDED
