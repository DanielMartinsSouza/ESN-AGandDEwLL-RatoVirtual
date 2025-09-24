cmake_minimum_required(VERSION 4.0)
project(ESN_AGandDEwLL_RatoVirtual)

set(CMAKE_CXX_STANDARD 20)

add_executable(ESN_AGandDEwLL_RatoVirtual main.cpp
        src/parameters/Parameters.cpp
        src/parameters/Parameters.h
        src/algorithms/AG/AG.cpp
        src/utils/saveFiles.cpp
        src/core/Random.cpp
        src/core/Random.h
        src/core/nr_ran.cpp
        src/core/nr_ran.h
        src/algorithms/AG/fitness.cpp
        src/core/Simulador.cpp
        src/core/Simulador.h
        src/utils/utils_functions.cpp
        src/core/ESN.cpp
        src/core/ESN.h
        src/algorithms/AG/estatistica.cpp
        src/algorithms/AG/geracao.cpp
        src/algorithms/AG/selecao.cpp
        src/algorithms/AG/reproducao.cpp
        src/core/estVIG2.cpp
        src/core/estVIG2.h
        src/algorithms/DE/estatistica.cpp
        src/algorithms/DE/reproducao.cpp
        src/algorithms/LinkageLearning/LL.cpp
        src/core/Graph.cpp
        src/core/Graph.h
        src/core/Graph.h
        src/core/Graph.cpp
        src/algorithms/DE/estatistica.cpp
        src/algorithms/DE/DE.cpp
)
