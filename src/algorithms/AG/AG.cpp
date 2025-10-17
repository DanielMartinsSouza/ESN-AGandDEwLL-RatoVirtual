//
// Created by Daniel on 19/09/2025.
//
#include <ctime>
#include <iostream>
#include <limits>
#include "../../parameters/Parameters.h"
using namespace std;

void impressao(const populacao *pop, int gen) {
    cout << "Geracao: " << gen << endl;
    cout << "Individuo com melhor fitness: " << pop->melhorIndividuo << endl;
    cout << "Fitness do melhor Individuo: " << pop->maxFitness << endl;
    cout << "Media do Fitness da geracao: " << pop->mediaFitness << endl
            << endl
            << endl;
}

void inicializacao(const int nroExec) {
    apaga_arquivos(nroExec);
    int numIndiv = 0;

    while (numIndiv < tamPop) {
        for (int gene = 0; gene < lcrom; gene++) {
            popVelha.indiv[numIndiv].cromossomo[gene] = random2->nextFloat(-1, 1);
        }
        popVelha.indiv[numIndiv].fitness = calcFitness(popVelha.indiv[numIndiv].cromossomo);
        // Armazena Fitness do Individuo
        numIndiv++;
    }
    estatistica(&popVelha, 0);
    impressao(&popVelha, 0);
}

void algGen(const int nroExec, const int seed) {
    int gen = 0;

    inicializacao(nroExec); // procedimento para inicializa  o das vari veis e da popula  o

    do {
        gen = gen + 1; // n mero de gera  es
        geracao();
        estatistica(&popNova, gen);

        individuo *aux = popVelha.indiv;
        popVelha = popNova;
        popNova.indiv = aux;

        impressao(&popVelha, gen);
    } while (gen < maxGen);
    arq_saida(nroExec, seed); // salva dados
}

int main_AG() {
    // Alocacao Dinamica de Memoria
    arq_media_fitness = aloc_vectord(maxGen + 1);
    arq_melhor_fitness = aloc_vectord(maxGen + 1);
    popVelha.indiv = aloc_vectorind(tamPop);
    popNova.indiv = aloc_vectorind(tamPop);
    for (int num_ind = 0; num_ind < tamPop; num_ind++) {
        popVelha.indiv[num_ind].cromossomo = aloc_vectord(lcrom);
        popNova.indiv[num_ind].cromossomo = aloc_vectord(lcrom);
    }
    arq_melhor_individuo = aloc_matrixd(maxGen + 1, lcrom);

    // Execucao
    cout << "***** Algoritmo Genetico *****" << endl;

    pX = new double[nsteps + 1];
    pY = new double[nsteps + 1];
    bestX = new double[nsteps + 1];
    bestY = new double[nsteps + 1];
    bestAcoes = new double[nsteps + 1];

    const int seed = time(nullptr);
    // ESN(int reservoir_size, double sparsity, double spectral_radius_d, int n_out, int input_size, int n_examples, int n_stab, int seed)
    esn = new ESN(reservoir_size, sparsity, spectral_radius_d, n_out, input_size, n_examples, n_stab, seed);

    for (int nroExec = 0; nroExec < nroMaxExec; nroExec++) {
        nfeval = 0;
        bestFitness = numeric_limits<double>::min(); // menor double negativo
        random2 = new Random(1, nroExec + 1); // semente para gerar os numeros aleatorios

        // Visualizacao
        cout << "\tExecucao: " << nroExec << endl
        //        << endl;
        algGen(nroExec, seed); // chama a execucao do AG para uma semente aleatoria

        delete random2;
    }

    cout << "Quantidade de fitness avaliado: " << nfeval << endl;

    /* Visualizacao */
    cout << "\tFim do programa!" << endl;

    // Desalocacao de Memoria
    delete[] pX;
    delete[] pY;
    delete[] bestX;
    delete[] bestY;
    delete[] bestAcoes;
    delete[] arq_media_fitness;
    delete[] arq_melhor_fitness;
    for (int num_ind = 0; num_ind < tamPop; num_ind++) {
        delete[] popVelha.indiv[num_ind].cromossomo;
        delete[] popNova.indiv[num_ind].cromossomo;
    }
    delete[] popVelha.indiv;
    delete[] popNova.indiv;
    delete esn;
    desaloc_matrixd(arq_melhor_individuo, maxGen + 1);

    return 0;
}