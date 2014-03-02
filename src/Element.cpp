#include "Globals.h"
#include "Level.h"
#include "TileSet.h"
#include "Element.h"

using namespace irr;

Element::Element(Level* level, Type type, unsigned id, irr::core::vector2di imgPosition, core::vector2df position)
 : m_level(level)
 , m_type(type)
 , m_id(id)
 , m_imgPosition(imgPosition)
 , m_position(position)
 , m_tileData(level->getTileSet()->getData(type, id))
{
    s32 unit = (s32)m_level->getUnitSize();
    m_boudingBox = {0, 0, unit, unit};
    m_level->addElement(this);
}

Element::~Element()
{
    m_level->removeElement(this);
}

Element::Type Element::getType() const
{
    return m_type;
}

unsigned Element::getId() const
{
    return m_id;
}

core::vector2di Element::getImagePosition() const
{
    return m_imgPosition;
}

core::vector2df Element::getPosition() const
{
    return m_position;
}

void Element::setPosition(core::vector2df position)
{
    m_position = position;
}

core::recti Element::getBoundingBox() const
{
    return m_boudingBox;
}

void Element::update()
{
}

void Element::draw()
{
    drawTile(m_level, m_tileData, m_imgPosition, m_position);
}

void Element::drawTile(Level* level, const TileData* td, core::vector2di imgPos, core::vector2df pos)
{
    core::rect<s32> srcRect =
        {(s32)(imgPos.X * td->tileSize), (s32)(imgPos.Y * td->tileSize),
        (s32)((imgPos.X + 1) * td->tileSize), (s32)((imgPos.Y + 1) * td->tileSize)};

    unsigned unit = level->getUnitSize();
    core::vector2di calcPos = {(s32)(pos.X * unit), (s32)(pos.Y * unit)};

    core::rect<s32> destRect = {0, 0, (s32)unit, (s32)unit};
    destRect += calcPos;
    destRect -= level->getViewOffset();

    level->getGlobals()->driver->draw2DImage(td->texture, destRect, srcRect, 0, 0, true);
}


std::istream& operator>> (std::istream& i, Element::Type& t)
{
    unsigned type;
    i >> type;
    t = static_cast<Element::Type>(type);
    return i;
}

std::istream& operator>> (std::istream& i, Element::Visibility& v)
{
    unsigned visibility;
    i >> visibility;
    v = static_cast<Element::Visibility>(visibility);
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2di& v)
{
    int x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}

std::istream& operator>> (std::istream& i, core::vector2df& v)
{
    float x,y;
    char c;
    i >> x >> c >> y;
    v.X = x;
    v.Y = y;
    return i;
}
