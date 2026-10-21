#pragma once

#include <vector>

#include "instancia.hpp"

class Evaluador {
    private:
        std::vector<int> aux_;
        long long eval_ = 0;

    public:
        Evaluador(const Instancia& inst);
        void reiniciarContador() {eval_ = 0;}
        const Instancia& inst_;
        int makespan(const std::vector<int>& perm);
        long long evaluaciones() const {return eval_;}
        const Instancia& instancia() const {return inst_;}
};