#ifndef MOTION_H_INCLUDED
#define MOTION_H_INCLUDED

#include <iostream>

enum class Motion : unsigned
{
    NONE = 0,
	STATIC = 3,
	UNSTABLE = 4,
    DYNAMIC = 5,
    AUTO = 6,
    STRAIGHT = 7,
    CIRCULAR = 8
};

std::istream& operator>> (std::istream&, Motion&);

#endif // MOTION_H_INCLUDED
