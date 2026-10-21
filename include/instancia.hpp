#pragma once

#include <fstream>
#include <vector>

class Instancia {
    private: 
        int trabajos_;
        int maquinas_;
        int semilla_;
        int cota_superior_;
        int cota_inferior_;

    public:
        std::vector<std::vector<int>> tiempos_;
        void show();
        int getTrabajos() const {return trabajos_;}
        int getMaquinas() const {return maquinas_;}
        int upperBound() const {return cota_superior_;}
        Instancia(std::ifstream& file);
        ~Instancia() = default;
};

