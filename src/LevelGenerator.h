#ifndef LEVELGENERATOR_H_INCLUDED
#define LEVELGENERATOR_H_INCLUDED

#include <cstdint>

class Level;
class TileSet;

class LevelGenerator
{
public:
    struct Params
    {
        uint32_t seed;
        double threshold;
        bool border;
        /* ... */
    };

    LevelGenerator();
    ~LevelGenerator();
    Params getDefaultParams() const;
    void generate(Level*, unsigned columns, unsigned rows, const Params* = nullptr);

private:
    Params m_defaultParams;
};

#endif // LEVELGENERATOR_H_INCLUDED
