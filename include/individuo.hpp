#pragma once

#include <climits>
#include <vector>

struct Individuo {
    std::vector<int> perm;
    int makespan = INT_MAX;
};