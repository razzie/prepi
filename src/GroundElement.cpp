#include "Globals.h"
#include "Level.h"
#include "GroundElement.h"

using namespace irr;

GroundElement::GroundElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df, Visibility, Motion> data)
 : GroundElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data),
    std::get<3>(data),
    std::get<4>(data))
{
}

GroundElement::GroundElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord,
                             Visibility visibility, Motion motion)
 : Element(level, Type::GROUND, id, position, realCoord)
 , m_visibility(visibility)
 , m_motion(motion)
{
    std::cout << "new GroundElement" << std::endl;
}

GroundElement::~GroundElement()
{
}

Element::Visibility GroundElement::getVisibility() const
{
    return m_visibility;
}

Motion GroundElement::getMotion() const
{
    return m_motion;
}

void GroundElement::update()
{
    unsigned unit = m_level->getUnitSize();
    core::recti box = {m_realCoord.X*unit, m_realCoord.Y*unit, m_realCoord.X*unit+unit, m_realCoord.Y*unit+unit};
    box -= m_level->getView().UpperLeftCorner;

    m_level->getGlobals()->driver->draw2DRectangle({255,0,255,0}, box);
}
