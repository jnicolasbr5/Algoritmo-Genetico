// referencia.h
#pragma once

#include <random>
#include <utility>
#include <vector>

#include "evaluador.hpp"
#include "resultado.hpp"


Resultado busquedaAleatoria(Evaluador& ev, int n, long long presupuesto, std::mt19937& rng);
