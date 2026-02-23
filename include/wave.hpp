#pragma once
#include "array2d.hpp"
#include <cstddef>
#include <map>


using WfcIds = std::map<int, float>;

/*
 * The WFC state, the wave function stores all superpositions for all tiles
 */
class Wave {
private:
    Array2D<std::map<int, bool>> state;

public:
    Wave(std::size_t width, std::size_t height, const WfcIds& ids);
};

