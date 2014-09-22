#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "irrlicht.h"
#include <iostream>

void randomizeColor(irr::video::SColor&, uint8_t randomness);
void randomizeColor(irr::video::SColor&, uint8_t randR, uint8_t randG, uint8_t randB);

std::istream& operator>> (std::istream&, irr::video::SColor&);

#endif // COLOR_H_INCLUDED
