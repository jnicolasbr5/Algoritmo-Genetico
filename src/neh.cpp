#include "neh.hpp"

#include <algorithm>
#include <numeric>  
#include <iostream>
#include "resultado.hpp"

Resultado neh(Evaluador& ev) {
    const Instancia& ins = ev.instancia();
    const int n = ins.getTrabajos(), m = ins.getMaquinas();

    // Tiempo total por trabajo
    std::vector<int> total(n, 0);
    for (int j = 0; j < n; j++) { // Cada trabajo
        for (int i = 0; i < m; i++) {   // Tiempo por máquina
            total[j] += ins.tiempos_[i][j];   
        }
    }

    // Ordeno la permutación de manera decreciente según los costes
    std::vector<int> orden(n);
    std::iota(orden.begin(), orden.end(), 0);
    std::stable_sort(orden.begin(), orden.end(), [&](int a, int b) { return total[a] > total[b]; });

    // Construcción por inserciones
    std::vector<int> sec;
    sec.push_back(orden[0]); // Añado el trabajo más pesado
    int mejorC = INT_MAX;

    for (int k = 1; k < n; k++) {
        int j = orden[k]; 
        mejorC = INT_MAX;
        int mejorPos = 0;

        // Se prueban las distintas evaluaciones
        for (int pos = 0; pos <= sec.size(); pos++) {
            sec.insert(sec.begin() + pos, j);
            int c = ev.makespan(sec);
            sec.erase(sec.begin() + pos);
            if (c < mejorC) { 
                mejorC = c; 
                mejorPos = pos; 
            }
        }
        sec.insert(sec.begin() + mejorPos, j);
    }

    Resultado r;
    r.mejor_.perm = sec;
    r.mejor_.makespan = mejorC;
    r.traza_.push_back({ev.evaluaciones(), r.mejor_.makespan});
    return r;
}