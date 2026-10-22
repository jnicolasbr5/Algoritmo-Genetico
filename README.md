# Práctica 2: Algoritmo genético para el PFSP (Permutation Flow Shop)

Inteligencia Artificial — 3.º Grado en Ingeniería Informática

## Contenido del proyecto

```
proyecto/
├── README.md
├── CMakeLists.txt
├── data/         Resultados globales (ejecución de todas las instancias)
├── images/       Imágenes de las gráficas
├── include/      Cabeceras (.hpp)
├── src/          Código fuente (.cpp)
├── scripts/      Scripts de bash y python
├── insts/        Instancias de Taillard (.fsp)
├── resultados/   Trazas de convergencia (CSV) y tabla rpd (.txt)
```

Algoritmos implementados:

| Algoritmo | Opción |
|---|---|
| Búsqueda aleatoria (referencia) | `--alg random` |
| Heurística NEH (referencia) | `--alg neh` |
| Algoritmo genético (AG) | `--alg ga` (por defecto) |

## Compilación

Desde la raíz del proyecto:

```bash
mkdir build
cd build
cmake .. -DCMAKE_CXX_COMPILER=/usr/bin/g++
make
```

El ejecutable queda en `build/genetic`.

## Ejecución

```bash
./build/genetic <instancia> [opciones]
```

Las opciones son opcionales y van en pares `--opción valor`:

| Opción | Descripción | Por defecto |
|---|---|---|
| `--alg ga` | Algoritmo a ejecutar | `ga` |
| `--seed S` | Semilla del generador aleatorio | `42` |
| `--pres E` | Evaluaciones del makespan | `50000` |
| `--N N` | Tamaño de la población | `100` |
| `--pc PC` | Probabilidad de cruce | `0.9` |
| `--pm PM` | Probabilidad de mutación (por hijo) | `0.1` |
| `--k K` | Tamaño del torneo | `2` |
| `--traza convergencia.csv` | Guarda la traza de convergencia 

Si algún argumento es incorrecto, el programa muestra el error y termina.

Ejemplos:

```bash
./build/genetic insts/tai20_5_0.fsp
./build/genetic insts/tai20_5_0.fsp --alg random --semilla 5
./build/genetic insts/tai50_20_0.fsp --alg ga --N 30 --pm 0.2 --semilla 3 --pc 0.4
```

Salida por pantalla:

```
Cmax = <makespan>  evaluaciones = <n>
perm (1..n): <secuencia de trabajos>
mejoras en la traza: <n>
```

Con la misma instancia, parámetros y semilla, el resultado es siempre el mismo. La traza es un CSV con columnas `evaluaciones,mejor_cmax`, que registra cada mejora de la mejor solución.

## Scripts

El script se debe lanzar desde su carpeta

| Script | Función |
|---|---|
| `scripts/conver.py` | Genera la gráfica de la instancia
| `scripts/script.sh` | Ejecuta todos los algoritmos


## Resultados y análisis

Los resultados globales están en `data/`.
Los resultados por ejecución están en `resultados/convergencia.csv` y el análisis completo en `resultados/rpd.txt`.