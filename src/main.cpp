#include <fstream>
#include <iostream>
#include <print>
#include <random>

#include "evaluador.hpp"
#include "instancia.hpp"
#include "referencia.hpp"
#include "resultado.hpp"

int main(int argc, char *argv[]) {
    // Leer la línea de comandos
    // Uso: ./t2a <instancia> [semilla] [presupuesto] [fichero_traza.csv]
    std::string ruta = (argc > 1) ? argv[1] : "prueba.txt";
    unsigned semilla = (argc > 2) ? std::stoul(argv[2]) : 42;
    long long presupuesto = (argc > 3) ? std::stoll(argv[3]) : 50000;
    std::string rutaTraza = (argc > 4) ? argv[4] : "traza.txt";

    // std::ifstream file("../go-taillard-master/pfsp/instances/tai20_5_0.fsp");

    // Inicializar objetos
    std::ifstream file(ruta);
    Instancia instancia(file);
    instancia.show();
    Evaluador eval(instancia);
    std::mt19937 rng(semilla);

    Resultado r = busquedaAleatoria(eval, instancia.getTrabajos(), presupuesto, rng);

    std::cout << "Cmax = " << r.mejor_.makespan << "  evaluaciones = " << eval.evaluaciones() << "\n";
    std::cout << "perm (1..n): ";
    for (int j : r.mejor_.perm) std::cout << j + 1 << " ";
    std::cout << "\nmejoras en la traza: " << r.traza_.size() << "\n";

    // Opcional: guardar la traza de convergencia en CSV 
    if (!rutaTraza.empty()) {
        std::ofstream f(rutaTraza);
        f << "evaluaciones,mejor_cmax\n";
        for (const auto& [ev_i, c] : r.traza_)
            f << ev_i << "," << c << "\n";
    }

    return 0;
}