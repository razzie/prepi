#include <stdexcept>
//#include <cctype>
#include "Color.h"

using namespace irr;

std::istream& operator>> (std::istream& stream, irr::video::SColor& color)
{
    char c[6];

    if (stream.peek() == '#') stream.ignore();

    if ((stream >> c[0])
        && (stream >> c[1])
        && (stream >> c[2])
        && (stream >> c[3])
        && (stream >> c[4])
        && (stream >> c[5]))
    {
        for (int i = 0; i < 6; ++i)
        {
            if (!(c[i] >= 'a' && c[i] <= 'f')
                && !(c[i] >= 'A' && c[i] <= 'F')
                && !(c[i] >= '0' && c[i]))
            {
                throw std::runtime_error("wrong color code");
            }
        }

        color.setRed(c[0]*255 + c[1]);
        color.setGreen(c[2]*255 + c[3]);
        color.setBlue(c[4]*255 + c[5]);
    }
    else
    {
        throw std::runtime_error("wrong color code");
    }

    return stream;
}
