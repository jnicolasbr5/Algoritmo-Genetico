#include <iostream>
#include <sstream>

#include "instancia.hpp"

Instancia::Instancia(std::ifstream& file) { 
    std::string line;
    std::getline(file, line); // Ignoro la primera línea
    std::getline(file, line); // Datos iniciales
    std::stringstream ss(line);
    int num;
    ss >> trabajos_ >> maquinas_ >> semilla_ >> cota_superior_ >> cota_inferior_;
    std::getline(file, line); // Ignora texto
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::vector<int> tiempos;
        for (int i = 0; i < trabajos_; i++) {
            ss >> num;
            tiempos.push_back(num);
        }
        tiempos_.push_back(tiempos);
    }
}

void Instancia::show() {
    for (int i = 0; i < maquinas_; i++) {
        for (int j = 0; j < trabajos_; j++) {
            std::cout << tiempos_[i][j] << " ";
        }
        std::cout << "\n";
    }
}