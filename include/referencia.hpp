// referencia.h
#pragma once
#include "evaluador.hpp"
#include "individuo.hpp"
#include <random>
#include <utility>
#include <vector>

// Cada par = (nº de evaluaciones, mejor makespan en ese momento)
using Convergencia = std::vector<std::pair<long, int>>;

Individuo busquedaAleatoria(Evaluador& ev, std::mt19937& rng, Convergencia& curva);

Individuo neh(Evaluador& ev);