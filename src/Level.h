#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <set>
#include <string>

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
    void update();

private:
    Globals* m_globals;
    std::set<Element*> m_elements;
    unsigned m_columns, m_rows;
    unsigned m_bgId;
    bgDrawingMethod m_bgDrawingMethod;
};

#endif // LEVEL_H_INCLUDED
