#ifndef MOTION_H_INCLUDED
#define MOTION_H_INCLUDED

#include <iostream>

enum class Motion : unsigned
{
	STATIC = 0,
    DYNAMIC = 1,
    AUTO = 2,
    SRAIGHT = 3,
    CIRCULAR = 4
};

std::istream& operator>> (std::istream&, Motion&);

#endif // MOTION_H_INCLUDED
