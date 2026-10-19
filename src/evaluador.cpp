#include "evaluador.hpp"

#include <iostream>
#include <print>

Evaluador::Evaluador(const Instancia& inst) : inst_(inst) {}

int Evaluador::makespan(const std::vector<int>& perm) {
    eval_++;
    const int m = inst_.getMaquinas();
    aux_.assign(m, 0);
    for (int trabajo : perm) {
        aux_[0] += inst_.tiempos_[0][trabajo];
        for (int i = 1; i < m; i++) {
            aux_[i] = std::max(aux_[i], aux_[i - 1]) + inst_.tiempos_[i][trabajo];
        }
    }
    return aux_[aux_.size() - 1];
}