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
    void setDimension(irr::core::dimension2du);
    irr::core::dimension2du getDimension() const;
    unsigned getUnitSize() const;
    void setView(irr::core::rect<irr::s32>);
    irr::core::rect<irr::s32> getView() const;
    void update();

private:
    mutable tthread::mutex m_mutex;
    Globals* m_globals;
    TileSet* m_tileset;
    irr::core::rect<irr::s32> m_view;
    irr::core::dimension2du m_dimension;
    unsigned m_unit;
    std::set<Element*> m_elements;
    Background* m_bg;
};

#endif // LEVEL_H_INCLUDED
