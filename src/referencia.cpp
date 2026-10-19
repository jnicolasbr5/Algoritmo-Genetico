#include "referencia.hpp"
#include <algorithm>
#include <climits>
#include <numeric>

Individuo busquedaAleatoria(Evaluador& ev, std::mt19937& rng, Convergencia& curva) {
    const int n = ev.instancia().n;

    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);      // 0,1,...,n-1

    Individuo mejor;
    mejor.makespan = INT_MAX;

    while (!ev.agotado()) {
        std::shuffle(perm.begin(), perm.end(), rng);   // permutación aleatoria
        int c = ev.makespan(perm);                      // cuenta 1 evaluación

        if (c < mejor.makespan) {
            mejor.perm = perm;
            mejor.makespan = c;
            curva.push_back({ev.evaluaciones(), c});
        }
    }
    return mejor;
}


Individuo neh(Evaluador& ev) {
    const Instancia& inst = ev.instancia();
    const int n = inst.n;

    // Paso 1: suma total por trabajo y orden decreciente
    std::vector<int> total(n, 0);
    for (int j = 0; j < n; ++j)
        for (int i = 0; i < inst.m; ++i)
            total[j] += inst.p[i][j];

    std::vector<int> orden(n);
    std::iota(orden.begin(), orden.end(), 0);
    std::stable_sort(orden.begin(), orden.end(),
                     [&](int a, int b) { return total[a] > total[b]; });

    // Pasos 2 y 3: inserción en la mejor posición
    std::vector<int> seq = {orden[0]};
    int mejorC = inst.n == 1 ? ev.makespan(seq) : INT_MAX;

    for (int k = 1; k < n; ++k) {
        int job = orden[k];
        int mejorPos = 0;
        mejorC = INT_MAX;

        for (int pos = 0; pos <= (int)seq.size(); ++pos) {
            seq.insert(seq.begin() + pos, job);   // probar en 'pos'
            int c = ev.makespan(seq);             // cuenta 1 evaluación
            if (c < mejorC) { mejorC = c; mejorPos = pos; }
            seq.erase(seq.begin() + pos);         // deshacer
        }
        seq.insert(seq.begin() + mejorPos, job);  // colocar definitivamente
    }

    Individuo r;
    r.perm = seq;
    r.makespan = mejorC;
    return r;
}