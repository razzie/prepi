#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <set>
#include <string>
#include "irrlicht.h"

class Globals;
class Element;

class Level
{
public:
    Level(Globals*, std::string url);
    Level(const Level&) = delete;
    ~Level();

    enum class bgDrawingMethod : unsigned
    {
        STRETCH = 0,
        VERTICAL = 1,
        HORIZONTAL = 2,
        TILE = 3
    };

    Globals* getGlobals();
    void addElement(Element*);
    void removeElement(Element*);
    void setBackground(unsigned id, bgDrawingMethod);
    void setDimension(unsigned columns, unsigned rows);
    void setView(irr::core::rect<irr::s32>);
    irr::core::rect<irr::s32> getView() const;
    void update();

protected:
    void drawBackground();

private:
    // displaying related stuff
    Globals* m_globals;
    std::set<Element*> m_elements;
    irr::core::rect<irr::s32> m_view;
    unsigned m_unit;
    // level dimensions
    unsigned m_columns, m_rows;
    // background related stuff
    irr::video::ITexture* m_bg;
    //irr::core::rect<irr::s32> m_bgSrcRect, m_bgDestRect;
    //unsigned m_bgXRepeats, m_bgYRepeats;
    bgDrawingMethod m_bgDrawingMethod;
};

#endif // LEVEL_H_INCLUDED
