#include "Motion.h"

std::istream& operator>> (std::istream& i, Motion& m)
{
    unsigned motion;
    i >> motion;

    switch (motion)
    {
        case 0: // BACK
        case 2: // FORE
            m = Motion::NONE;
            break;

        case 3: // STATIC
        case 4: // UNSTABLE
        case 5: // DYNAMIC
        case 6: // AUTO
        case 7: // STRAIGHT
        case 8: // CIRCULAR
            m = static_cast<Motion>(motion);
            break;

        default:
            m = Motion::STATIC;
    }

    return i;
}
