#include <ctime>
#include <stdexcept>
#include "simplex.h"
#include "Level.h"
#include "TileSet.h"
#include "LevelGenerator.h"
#include "Background.h"
#include "Element.h"
#include "GroundElement.h"
#include "EnemyElement.h"
#include "RewardElement.h"
#include "PlayerElement.h"
#include "FinishElement.h"

LevelGenerator::LevelGenerator()
{
    srand(time(NULL));
    m_defaultParams.seed = rand() * rand();
    m_defaultParams.threshold = 0.5f;
    m_defaultParams.border = true;
}

LevelGenerator::~LevelGenerator()
{
}

LevelGenerator::Params LevelGenerator::getDefaultParams() const
{
    return m_defaultParams;
}

void LevelGenerator::generate(Level* level, unsigned columns, unsigned rows, const Params* params) const
{
    if (params == nullptr) params = &m_defaultParams;

    if (columns < 3 || rows < 3)
        throw std::runtime_error("column or row number is too low");


}
