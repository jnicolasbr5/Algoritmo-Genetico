#include <fstream>
#include <iostream>
#include <print>
#include <random>

#include "evaluador.hpp"
#include "instancia.hpp"
#include "neh.hpp"
#include "referencia.hpp"
#include "resultado.hpp"

int main(int argc, char *argv[]) {
    // Leer la línea de comandos
    // Uso: ./t2a <instancia> [semilla] [presupuesto] [fichero_traza.csv]
    std::string ruta = (argc > 1) ? argv[1] : "prueba.txt";
    std::string algoritmo = (argc > 2) ? argv[2] : "random";
    unsigned semilla = (argc > 3) ? std::stoul(argv[3]) : 42;
    long long presupuesto = (argc > 4) ? std::stoll(argv[4]) : 50000;
    std::string rutaTraza = (argc > 5) ? argv[5] : "traza.txt";

    // std::ifstream file("../go-taillard-master/pfsp/instances/tai20_5_0.fsp");

    // Inicializar objetos
    std::ifstream file(ruta);
    Instancia instancia(file);
    Evaluador eval(instancia);

    Resultado r;
    if (algoritmo == "neh") {
        std::println("---Uso del algoritmo NEH.---");
        r = neh(eval);
    }   else {
        std::println("---Uso del algoritmo de búsqueda aleatoria.---");
        std::mt19937 rng(semilla);
        r = busquedaAleatoria(eval, instancia.getTrabajos(), presupuesto, rng);
    }

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