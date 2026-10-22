#include "genetico.hpp"

#include <algorithm>
#include <cassert>  
#include <numeric>
#include <random>

#include "evaluador.hpp"
#include "individuo.hpp"
#include "neh.hpp"
#include "resultado.hpp"


Genetico::Genetico(Evaluador& eval, ParametrosAG param, std::string ruta, int& valor_neh) : eval_(eval), param_(param), conv_(ruta, std::ios::trunc), valor_neh_(valor_neh) {
    n_trabajos_ = eval_.inst_.getTrabajos();
    rng_.seed(param_.semilla);
    conv_ << "evaluaciones,makespan\n";
}


/**
 * @brief 
 * 
 * @param ind 
 */
void Genetico::evaluar(Individuo& ind) {
    ind.makespan = eval_.makespan(ind.perm);

    if (ind.makespan < res_.mejor_.makespan) {
        res_.mejor_ = ind;
        res_.traza_.push_back({eval_.evaluaciones(), ind.makespan});
    }

    // Muestra la evaluación y el makespan
    conv_ << eval_.evaluaciones() << "," << res_.mejor_.makespan << "\n";
}


/**
 * @brief Crea y evalúa la población inicial
 * 
 * El primer individuo es la solución de la heurística NEH y el resto son aleatorios.
 * 
 */
void Genetico::inicializarPoblacion() {
    poblacion_.assign(param_.N_, Individuo());

    // El primer individuo sera el valor NEH
    Resultado rneh = neh(eval_);
    valor_neh_ = rneh.mejor_.makespan;
    eval_.reiniciarContador();
    poblacion_[0] = rneh.mejor_;
    if (rneh.mejor_.makespan < res_.mejor_.makespan) {
            res_.mejor_ = rneh.mejor_;
            res_.traza_.push_back({eval_.evaluaciones(), rneh.mejor_.makespan});
    }

    // Búsqueda aleatoria
    for (int i = 1; i < param_.N_; i++) {
        Individuo& individuo = poblacion_[i];
        individuo.perm.resize(n_trabajos_);
        std::iota(individuo.perm.begin(), individuo.perm.end(), 0);
        std::shuffle(individuo.perm.begin(), individuo.perm.end(), rng_);
        evaluar(individuo);
    }   
}


/**
 * @brief Selección por torneo
 * 
 * Elige al azar k individuos y devuelve el índice del que tenga menor makespan
 * 
 * @return int Índice del individuo ganador del torneo
 */
int Genetico::seleccionar() {
    std::uniform_int_distribution<int> dist(0, param_.N_ - 1);
    int mejor = dist(rng_);  
    for (int i = 1; i < param_.torneo_; i++) {
        int c = dist(rng_);          
        if (poblacion_[c].makespan < poblacion_[mejor].makespan)
            mejor = c;
    }
    return mejor; 
}


/**
 * @brief Genera un hijo que conserva el segmento [a, b] de p1 en las mismas posiciones
 * y completa el resto con los 
 * 
 * @param p1 Primer padre: aporta el segmento
 * @param p2 Segundo padre: aporta el orden de los trabajos restantes
 * @param a Inicio del segmento a copiar  
 * @param b Fin del segmento a copiar
 * @return std::vector<int> Permutación hija del cruce
 */
std::vector<int> Genetico::cruceOX(const std::vector<int>& p1, const std::vector<int>& p2, int a, int b) {
    const int n = static_cast<int>(p1.size());
    std::vector<int> hijo(n, -1);          // -1 = hueco libre
    std::vector<bool> usado(n, false);     // trabajos ya presentes en el hijo

    // 1. Copiar el segmento de p1 en las mismas posiciones
    for (int i = a; i <= b; i++) {
        hijo[i] = p1[i];
        usado[p1[i]] = true;
    }

    // 2. Recorrer p2 y rellenar los huecos de izquierda a derecha
    int pos = 0;
    for (int i = 0; i < n; i++) {
        int trabajo = p2[i];
        if (usado[trabajo]) continue;      // ya está en el segmento
        while (hijo[pos] != -1) pos++;     // siguiente hueco libre
        hijo[pos] = trabajo;
    }
    return hijo;
}


void Genetico::mutacionSwap(std::vector<int>& perm, std::mt19937& rng) {
    std::uniform_int_distribution<int> d(0, (int)perm.size() - 1);
    int i = d(rng), j = d(rng);
    std::swap(perm[i], perm[j]);
}


void Genetico::mutacionInsercion(std::vector<int>& perm, std::mt19937& rng) {
    std::uniform_int_distribution<int> d(0, (int)perm.size() - 1);
    int i = d(rng), j = d(rng);
    int trabajo = perm[i];
    perm.erase(perm.begin() + i);              // lo saco de la posición i
    perm.insert(perm.begin() + j, trabajo);    // lo meto en la posición j
}


bool Genetico::esPermutacionValida(const std::vector<int>& v, int n) {
    if ((int)v.size() != n) return false;
    std::vector<bool> visto(n, false);
    for (int x : v) {
        if (x < 0 || x >= n || visto[x]) return false;
        visto[x] = true;
    }
    return true;
}


void Genetico::elegirMutacion(std::vector<int>& perm) {
    std::uniform_real_distribution<double> U(0.0, 1.0);
    if (U(rng_) < 0.5)
        mutacionInsercion(perm, rng_);
    else
        mutacionSwap(perm, rng_);
}   


/**
 * @brief Repite el ciclo selección → cruce → mutación → evaluación hasta reunir N hijos 
 * 
 * @return std::vector<Individuo> 
 */
std::vector<Individuo> Genetico::generarNuevaPoblacion() {
    std::uniform_real_distribution<double> U(0.0, 1.0); // Para probabilidades
    std::uniform_int_distribution<int> corte(0, n_trabajos_ - 1); // Para puntos del corte del cruce

    std::vector<Individuo> nueva;
    nueva.reserve(param_.N_);

    while ((int)nueva.size() < param_.N_) {
        if (eval_.evaluaciones() >= param_.presupuesto) break;

        // Selección por torneo de los dos padres
        const Individuo& p1 = poblacion_[seleccionar()];
        const Individuo& p2 = poblacion_[seleccionar()];

        Individuo h1, h2;
        if (U(rng_) < param_.pc) {
            int a = corte(rng_), b = corte(rng_); // Se eligen los puntos de corte y se ordenan
            if (a > b) std::swap(a, b);

            // h1 hereda el segmento de p1, y h2 el de p2
            h1.perm = cruceOX(p1.perm, p2.perm, a, b);
            h2.perm = cruceOX(p2.perm, p1.perm, a, b);
        } else {
            // Sin cruce: los hijos son copias de los padres
            h1.perm = p1.perm;
            h2.perm = p2.perm;
        }

        // Mutación, evaluación e inserción de cada hijo
        for (Individuo* h : {&h1, &h2}) {

            // Con probabilidad pm se aplica mutación 
            if (U(rng_) < param_.pm) {
                elegirMutacion(h->perm);
            }
            assert(esPermutacionValida(h->perm, n_trabajos_));
            if (eval_.evaluaciones() >= param_.presupuesto) break;
            evaluar(*h); // Calcula el makespan
            nueva.push_back(*h);
            if ((int)nueva.size() >= param_.N_) break;
        }
    }
    return nueva;
}


/**
 * @brief Reemplaza la población actual por la nueva aplicando elitismo.
 * 
 * @param nueva Población de hijos evaluados
 */
void Genetico::reemplazar(std::vector<Individuo>& nueva) {

    // Mejor individuo de la generación anterior
    auto mejorAnt = std::min_element(poblacion_.begin(), poblacion_.end(),
        [](const Individuo& x, const Individuo& y){ return x.makespan < y.makespan; });

    // Peor individuo de la nueva generación
    auto peorHijo = std::max_element(nueva.begin(), nueva.end(),
        [](const Individuo& x, const Individuo& y){ return x.makespan < y.makespan; });

    // Elitismo: si el mejor anterior supera al peor hijo, ocupa su lugar
    if (mejorAnt->makespan < peorHijo->makespan)
        *peorHijo = *mejorAnt;
    poblacion_ = std::move(nueva);
}


/**
 * @brief Ejecuta el algoritmo genético hasta agotar el presupuesto de evaluaciones.
 * 
 * Inicializa la población y, en cada generación, crea una nueva población de
 * hijos y la sustituye por la actual. Si el presupuesto se agota antes de
 * completar una generación, esa población incompleta se descarta.
 * 
 * @return Resultado Mejor resultado encontrado en la evaluación
 */
Resultado Genetico::ejecutar() {
    eval_.reiniciarContador();
    inicializarPoblacion();

    while (eval_.evaluaciones() < param_.presupuesto) {
        std::vector<Individuo> nueva = generarNuevaPoblacion();
        if ((int)nueva.size() == param_.N_)
            reemplazar(nueva);
    }
    return res_;
}