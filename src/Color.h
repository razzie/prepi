#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include "irrlicht.h"
#include <iostream>

std::istream& operator>> (std::istream&, irr::video::SColor&);

#endif // COLOR_H_INCLUDED
