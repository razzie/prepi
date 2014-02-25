#include "Globals.h"
#include "Level.h"
#include "FinishElement.h"

using namespace irr;

FinishElement::FinishElement(Level* level, std::tuple<unsigned, irr::core::vector2di, irr::core::vector2df> data)
 : FinishElement(level,
    std::get<0>(data),
    std::get<1>(data),
    std::get<2>(data))
{
}

FinishElement::FinishElement(Level* level, unsigned id,
                             irr::core::vector2di position, irr::core::vector2df realCoord)
 : Element(level, Type::FINISH, id, position, realCoord)
{
}

FinishElement::~FinishElement()
{
}

void FinishElement::draw()
{
    core::recti box = getBoundingBox();
    box -= m_level->getView().UpperLeftCorner;

    m_level->getGlobals()->driver->draw2DRectangle({255,255,255,255}, box);
}
