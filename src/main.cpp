#include <filesystem>
#include <fstream>
#include <iomanip>
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

    std::ifstream file(ruta);
    Instancia instancia(file);
    Evaluador eval(instancia);

    Resultado r;
    int valor_neh = 0;
    if (algoritmo == "neh") {
        std::println("---Uso del algoritmo NEH.---");
        r = neh(eval);
    }   else if (algoritmo == "random") {
        std::println("---Uso del algoritmo de búsqueda aleatoria.---");
        std::mt19937 rng(param.semilla);
        r = busquedaAleatoria(eval, instancia.getTrabajos(), param.presupuesto, rng);
    }   else if (algoritmo == "ga") {
        std::println("---Algoritmo genetico---");
        Genetico ga(eval, param, rutaTraza, valor_neh);
        r = ga.ejecutar();
    }   else {
        return 1;
    }

    std::cout << "Cmax = " << r.mejor_.makespan << "  evaluaciones = " << eval.evaluaciones() << "\n";
    std::cout << "perm (1..n): ";
    for (int j : r.mejor_.perm) std::cout << j + 1 << " ";
    std::cout << "\nmejoras en la traza: " << r.traza_.size() << "\n";

    ruta = std::filesystem::path(ruta).stem().string();

    std::string ruta_rpd = "../resultados/rpd.txt";
    std::ofstream rpd(ruta_rpd, std::ios::app);

    // Datos de la ejecución
    rpd << "evaluaciones=" << eval.evaluaciones() << ", pc=" << param.pc << ", pm=" << param.pm << ", k=" << param.torneo_ << ", poblacion=" << param.N_ << "\n";
    
    // Columnas
    rpd << std::left
        << std::setw(15) << "Instancia" << "|"
        << std::setw(5)  << "n"         << "|"
        << std::setw(5)  << "m"         << "|"
        << std::setw(7)  << "NEH"       << "|"
        << std::setw(7)  << "Cmax"      << "|"
        << std::setw(6)  << "RPD"       << "|"
        << "RPDNEH\n"; 

    // Datos de las columnas    
    rpd << std::left
        << std::setw(15) << ruta << "|"
        << std::setw(5) << instancia.getTrabajos() << "|"
        << std::setw(5) << instancia.getMaquinas() << "|"
        << std::setw(7) << valor_neh << "|"
        << std::setw(7) << r.mejor_.makespan << "|"
        << std::fixed << std::setprecision(2)
        << std::setw(6) << RPD(r.mejor_.makespan, instancia.upperBound()) << "|"
        << std::fixed << std::setprecision(2)
        << std::setw(6) << RPD(valor_neh, instancia.upperBound()) << "\n\n";

    return 0;
}