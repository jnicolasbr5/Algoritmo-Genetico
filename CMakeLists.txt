cmake_minimum_required(VERSION 3.20)

# Project name and language
project(socket VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include folder
include_directories(include)

# List all source files
set(SOURCES
    src/main.cpp
    src/instancia.cpp
    src/evaluador.cpp
    src/busqueda_aleatoria.cpp
    src/neh.cpp
    src/genetico.cpp
)

# Create executable
add_executable(genetic ${SOURCES})

# Optional: compiler warnings
target_compile_options(genetic PRIVATE -Wall -Wextra -Wpedantic)

