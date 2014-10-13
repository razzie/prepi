#ifndef TEXTBEHAVIOR_H_INCLUDED
#define TEXTBEHAVIOR_H_INCLUDED

#include <tuple>
#include <string>
#include "irrlicht.h"
#include "behaviors\Behavior.h"

class Level;

class TextBehavior : public Behavior
{
public:
    TextBehavior(Element*, std::istream&);
    TextBehavior(Element*, std::tuple<std::string, irr::video::SColor>);
    TextBehavior(Element*, std::string, irr::video::SColor);
    ~TextBehavior();
    //void setElement(Element*);
    void update(uint32_t);

protected:
    Level* m_level;
    std::string m_text;
    irr::video::SColor m_color;
    bool m_triggered;
};

#endif // TEXTBEHAVIOR_H_INCLUDED
