#include <fstream>
#include <print>

#include "evaluador.hpp"
#include "instancia.hpp"

int main() {
    // std::ifstream file("../go-taillard-master/pfsp/instances/tai20_5_0.fsp");
    std::ifstream file("prueba.txt");
    Instancia instancia(file);
    instancia.show();
    Evaluador eval(instancia);
    int a = eval.makespan({0, 1, 2, 3});
    int b = eval.makespan({2, 3, 1, 0});

    std::println("Primero: {}, segundo: {}, eval: {}", a, b, eval.evaluaciones());
    /*

for (int semilla = 0; semilla < 10; ++semilla) {
    Evaluador ev(inst, 50000);          // contador nuevo en cada ejecución
    std::mt19937 rng(semilla);          // semilla distinta en cada ejecución
    Convergencia curva;

    Individuo r = busquedaAleatoria(ev, rng, curva);
    // guardar en CSV: instancia, "aleatoria", semilla, r.makespan
}
    */
    return 0;
}