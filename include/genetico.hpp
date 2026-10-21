#pragma once

#include <random>
#include <vector>

#include "evaluador.hpp"
#include "individuo.hpp"
#include "resultado.hpp"

struct ParametrosAG {
    int N_ = 100; // Poblacion size
    double pc = 0.9; // Probabilidad de cruce
    double pm = 0.1; // Probabilidad de mutación 
    int torneo_ = 2; // Tamaño del torneo
    int semilla = 42;
    long long presupuesto = 50000;
};

class Genetico {
    private:
        Evaluador& eval_;
        ParametrosAG param_;
        int n_trabajos_;
        std::mt19937 rng_;
        std::vector<Individuo> poblacion_;
        Resultado res_;

        void evaluar(Individuo& ind);
        void inicializarPoblacion();
        
        int seleccionar();
        std::vector<int> cruceOX(const std::vector<int>& p1, const std::vector<int>& p2, int a, int b);

        std::vector<Individuo> generarNuevaPoblacion();
        void reemplazar(std::vector<Individuo>& nueva);

        void mutacionSwap(std::vector<int>& perm, std::mt19937& rng);
        void mutacionInsercion(std::vector<int>& perm, std::mt19937& rng);
        bool esPermutacionValida(const std::vector<int>& v, int n);
        void elegirMutacion(std::vector<int>& perm);

    public:
        Genetico(Evaluador& eval, ParametrosAG param);
        ~Genetico() = default;

        Resultado ejecutar();
        
};