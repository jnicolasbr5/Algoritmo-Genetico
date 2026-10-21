# Práctica 2: Algoritmo genético para el PFSP (Permutation Flow Shop)

Inteligencia Artificial — 3.º Grado en Ingeniería Informática

## 1. Contenido del proyecto

```
proyecto/
├── README.md
├── CMakeLists.txt
├── include/      Cabeceras (.hpp)
├── src/          Código fuente (.cpp)
├── scripts/      Scripts de experimentación (ajuste de parámetros y T6)
├── insts/        Instancias de Taillard (.fsp). Colocarlas aquí
├── resultados/   Resultados brutos (CSV) de todas las ejecuciones
├── trazas/       Trazas de convergencia (CSV)
└── docs/
    └── informe.txt   Ajuste, resultados y análisis (T5, T6, T7)
```

Algoritmos implementados:

| Algoritmo | Opción |
|---|---|
| Búsqueda aleatoria (referencia) | `--alg random` |
| Heurística NEH (referencia) | `--alg neh` |
| Algoritmo genético (AG) | `--alg ga` (por defecto) |

## 2. Compilación

Desde la raíz del proyecto:

```bash
mkdir build
cd build
cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++
make
```

El ejecutable queda en `build/genetic`.

## 3. Ejecución

```bash
./build/genetic <instancia> [opciones]
```

Las opciones son opcionales y van en pares `--opción valor`:

| Opción | Descripción | Por defecto |
|---|---|---|
| `--alg ga\|neh\|random` | Algoritmo a ejecutar | `ga` |
| `--semilla S` | Semilla del generador aleatorio | `42` |
| `--presupuesto E` | Evaluaciones del makespan | `50000` |
| `--N N` | Tamaño de la población | `100` |
| `--pc PC` | Probabilidad de cruce | `0.9` |
| `--pm PM` | Probabilidad de mutación (por hijo) | `0.1` |
| `--torneo K` | Tamaño del torneo | `2` |
| `--traza fichero.csv` | Guarda la traza de convergencia | no se guarda |

Si algún argumento es incorrecto, el programa muestra el error y termina.

Ejemplos:

```bash
./build/genetic insts/tai20_5_0.fsp
./build/genetic insts/tai20_5_0.fsp --alg neh
./build/genetic insts/tai20_5_0.fsp --alg random --semilla 5
./build/genetic insts/tai50_20_0.fsp --alg ga --N 30 --pm 0.2 --semilla 3 --traza t.csv
```

Salida por pantalla:

```
Cmax = <makespan>  evaluaciones = <n>
perm (1..n): <secuencia de trabajos>
mejoras en la traza: <n>
```

Con la misma instancia, parámetros y semilla, el resultado es siempre el mismo (reproducible). La traza es un CSV con columnas `evaluaciones,mejor_cmax`, que registra cada mejora de la mejor solución.

## 4. Scripts

Todos los scripts hacen `cd` a la raíz del proyecto, así que pueden lanzarse desde cualquier carpeta.

| Script | Función |
|---|---|
| `scripts/ajuste.sh` | Ajuste de parámetros (T5): prueba N y pm sobre las instancias de ajuste con 5 semillas |
| `scripts/<T6>.sh` | Experimentación final (T6): NEH, aleatoria y AG con 10 semillas |

```bash
chmod +x scripts/ajuste.sh
./scripts/ajuste.sh | tee resultados/ajuste.txt
```

## 5. Decisiones de implementación

- **Representación:** permutación de los trabajos (índices 0..n-1 internamente; se muestran de 1..n por pantalla).
- **Makespan:** recurrencia (1) con un único vector de tamaño m, coste O(nm). El contador de evaluaciones vive dentro del `Evaluador`.
- **Selección:** torneo de tamaño k (por defecto 2), con reposición.
- **Cruce:** OX, versión de **relleno de izquierda a derecha** (la de la figura 3 del enunciado). Los dos hijos usan los mismos puntos de corte.
- **Mutación:** se aplica por hijo con probabilidad `pm`. Al mutar, se elige con probabilidad 0.5 entre inserción y swap.
- **Reemplazo:** generacional con elitismo (el mejor de la población anterior sustituye al peor hijo si es mejor que él).
- **Población inicial:** aleatoria (`shuffle` sobre la identidad).
- **Parada:** el presupuesto se comprueba antes de cada evaluación, así que el AG y la búsqueda aleatoria hacen exactamente 50 000 evaluaciones.
- **NEH:** orden decreciente de tiempo total con `std::stable_sort`; en caso de empate al insertar gana la primera posición probada. Es determinista (una ejecución por instancia) y hace n(n+1)/2 − 1 evaluaciones.
- **Factibilidad (T4):** `assert(esPermutacionValida(...))` tras cada cruce y mutación (activo solo en `Debug`).

## 6. Configuración final del AG

`N = 30`, `pm = 0.2`, `pc = 0.9`, `torneo = 2`, `presupuesto = 50000`.

La justificación está en `docs/informe.txt`.

> Nota: revisar si tras probar `pm = 0.4` la configuración cambia.

## 7. Resultados y análisis

Los resultados brutos están en `resultados/` y el análisis completo (T5, T6 y T7) en `docs/informe.txt`.