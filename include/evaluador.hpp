#pragma once

#include <vector>

#include "instancia.hpp"

class Evaluador {
    private:
        const Instancia& inst_;
        std::vector<int> aux_;
        long long eval_ = 0; 


    public:
        Evaluador(const Instancia& inst);
        int makespan(const std::vector<int>& perm);
        long long evaluaciones() const {return eval_;}
};