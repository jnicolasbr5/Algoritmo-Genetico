#include "busqueda_aleatoria.hpp"
#include "resultado.hpp"

#include <algorithm>
#include <climits>
#include <numeric>

Resultado busquedaAleatoria(Evaluador& ev, int n, long long presupuesto, std::mt19937& rng) {
    Resultado result;
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);

    while (ev.evaluaciones() < presupuesto) {
        std::shuffle(perm.begin(), perm.end(), rng);
        int cmax = ev.makespan(perm);
        if (cmax < result.mejor_.makespan) {
            result.mejor_.makespan = cmax;
            result.mejor_.perm = perm;
            result.traza_.push_back({ev.evaluaciones(), cmax});
        }
    }
    return result;
}