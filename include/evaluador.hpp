#pragma once

#include <vector>

#include "instancia.hpp"

class Evaluador {
    private:
        const Instancia& inst_;
        std::vector<int> aux_;
        int eval_ = 0; 


    public:
        Evaluador(const Instancia& inst);
        int makespan(const std::vector<int>& perm);
        int evaluaciones() const {return eval_;}
};

/*
// evaluador.h
#pragma once
#include "instancia.h"
#include <vector>

class Evaluador {
public:
    Evaluador(const Instancia& inst, long presupuesto)
        : inst_(inst), presupuesto_(presupuesto) {}

    int makespan(const std::vector<int>& perm);  // suma 1 al contador
    long evaluaciones() const { return contador_; }
    bool agotado() const { return contador_ >= presupuesto_; }

private:
    const Instancia& inst_;
    long contador_ = 0;
    long presupuesto_;
    std::vector<int> C_;   // vector auxiliar de tamaño m, reutilizado
};

*/