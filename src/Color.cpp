#include <ctime>
#include <stdexcept>
#include "Color.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define BETWEEN(x, min, max) ( MAX(0, MIN(255, (x))) )

using namespace irr;

void randomizeColor(irr::video::SColor& color, uint8_t randomness)
{
    randomizeColor(color, randomness, randomness, randomness);
}

void randomizeColor(irr::video::SColor& color, uint8_t randR, uint8_t randG, uint8_t randB)
{
#define RAND_VALUE(interval) ((rand() % BETWEEN((interval), 1, 255)) - (BETWEEN((interval), 1, 255) / 2))
    int R = color.getRed() + RAND_VALUE(randR);
    int G = color.getGreen() + RAND_VALUE(randG);
    int B = color.getBlue() + RAND_VALUE(randB);
    color.setRed( BETWEEN(R, 0, 255) );
    color.setGreen( BETWEEN(G, 0, 255) );
    color.setBlue( BETWEEN(B, 0, 255) );
#undef RAND_VALUE
}

std::istream& operator>> (std::istream& stream, irr::video::SColor& color)
{
    char c[6];
    uint8_t n[6];

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
            if (c[i] >= 'a' && c[i] <= 'f')
            {
                n[i] = c[i] - 'a' + 10;
            }
            else if (c[i] >= 'A' && c[i] <= 'F')
            {
                n[i] = c[i] - 'A' + 10;
            }
            else if (c[i] >= '0' && c[i])
            {
                n[i] = c[i] - '0';
            }
            else
            {
                throw std::runtime_error("wrong color code");
            }
        }

        color.setAlpha(255);
        color.setRed(n[0]*16 + n[1]);
        color.setGreen(n[2]*16 + n[3]);
        color.setBlue(n[4]*16 + n[5]);
    }
    else
    {
        throw std::runtime_error("wrong color code");
    }

    return stream;
}
