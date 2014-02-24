#include "Motion.h"

std::istream& operator>> (std::istream& i, Motion& m)
{
    unsigned motion;
    i >> motion;
    m = static_cast<Motion>(motion);
    return i;
}
