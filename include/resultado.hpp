#pragma once

#include <utility>
#include <vector>

#include "individuo.hpp"

struct Resultado {
    Individuo mejor_;
    std::vector<std::pair<long long, int>> traza_;
};