#include <fstream>
#include <iostream>
#include <print>
#include <random>

#include "evaluador.hpp"
#include "genetico.hpp"
#include "instancia.hpp"
#include "neh.hpp"
#include "busqueda_aleatoria.hpp"
#include "resultado.hpp"

void mostrarError() {
    std::cerr << "Uso: " << "./genetic <instancia> [--alg ga|neh|random] [opciones]\n";
}

double RPD(int makespan, int UB) {
    return 100.0 * (makespan - UB) / UB;
}

int main(int argc, char *argv[]) {
    // Leer la línea de comandos
    // Uso: ./t2a <instancia> [semilla] [presupuesto] [fichero_traza.csv]
    // ../inst/tai20_5_0.fsp
    // Falta de comandos
    if (argc < 2) {
        mostrarError();
        return 1;
    }

    // Valores de configuración predeterminados
    std::string ruta = argv[1];
    std::string algoritmo = "ga";
    std::string rutaTraza = "../resultados/convergencia.csv";
    ParametrosAG param;

    // Lectura de la línea de comandos
    for (int i = 2; i + 1 < argc; i += 2) {
        std::string op = argv[i];
        std::string val = argv[i + 1];
        if      (op == "--alg")         algoritmo = val;
        else if (op == "--traza")       rutaTraza = val;
        else if (op == "--N")           param.N_ = std::stoi(val);
        else if (op == "--pc")          param.pc = std::stod(val);
        else if (op == "--pm")          param.pm = std::stod(val);
        else if (op == "--k")      param.torneo_ = std::stoi(val);
        else if (op == "--seed")     param.semilla = std::stoi(val);
        else if (op == "--pres") param.presupuesto = std::stoll(val);
        else {
            std::cerr << "Opcion desconocida: " << op << "\n";
            return 1;
        }
    }

    // std::ifstream file("../go-taillard-master/pfsp/instances/tai20_5_0.fsp");

    // Inicializar objetos
    std::ifstream file(ruta);
    Instancia instancia(file);
    Evaluador eval(instancia);

    Resultado r;
    if (algoritmo == "neh") {
        std::println("---Uso del algoritmo NEH.---");
        r = neh(eval);
    }   else if (algoritmo == "random") {
        std::println("---Uso del algoritmo de búsqueda aleatoria.---");
        std::mt19937 rng(param.semilla);
        r = busquedaAleatoria(eval, instancia.getTrabajos(), param.presupuesto, rng);
    }   else if (algoritmo == "ga") {
        std::println("---Algoritmo genetico---");
        Genetico ga(eval, param);
        r = ga.ejecutar();
    }   else {
        return 1;
    }

    std::cout << "Cmax = " << r.mejor_.makespan << "  evaluaciones = " << eval.evaluaciones() << "\n";
    std::cout << "perm (1..n): ";
    for (int j : r.mejor_.perm) std::cout << j + 1 << " ";
    std::cout << "\nmejoras en la traza: " << r.traza_.size() << "\n";

    // Guardar la traza de convergencia en CSV 
    std::ofstream f(rutaTraza, std::ios::app);
    f.seekp(0, std::ios::end);
    if (f.tellp() == 0) {
        f << "instancia,semilla,evaluaciones,mejor_cmax\n";
    }
    for (const auto& [ev_i, c] : r.traza_)
        f << ruta << "," << param.semilla << "," << ev_i << "," << c << "\n";

    // Fichero CSV
    std::string resultados = "../resultados/resultados.csv";
    std::ofstream f2(resultados, std::ios::app);
    f2.seekp(0, std::ios::end);
    if (f2.tellp() == 0) {
        f2 << "instancia,n,m,semilla,makespan,UB,RPD,evaluaciones\n";
    }
    f2 << ruta << "," << instancia.getTrabajos() << "," << instancia.getMaquinas() << "," << param.semilla << ","
        << r.mejor_.makespan << "," << instancia.upperBound() << "," << RPD(r.mejor_.makespan, instancia.upperBound()) << "," << eval.evaluaciones() << "\n";
    
    return 0;
}