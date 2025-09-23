//
// Created by Daniel on 19/09/2025.
//
#include "../../parameters/Parameters.h"

void geracao()
{

    int j = 0;

    do
    {
        // Escolha dos Pais
        const int pai1 = selecao(&popVelha, j);
        const int pai2 = selecao(&popVelha, j + 1);

        // Reproducao
        crossover(popVelha.indiv[pai1].cromossomo, popVelha.indiv[pai2].cromossomo, popNova.indiv[j].cromossomo, popNova.indiv[j + 1].cromossomo, j);

        // Filho j
        popNova.indiv[j].fitness = calcFitness(popNova.indiv[j].cromossomo);
        popNova.indiv[j].pai1 = pai1; // pai 1 do filho j
        popNova.indiv[j].pai2 = pai2; // pai 2 do filho j
        // Filho j + 1
        popNova.indiv[j + 1].fitness = calcFitness(popNova.indiv[j + 1].cromossomo);
        popNova.indiv[j + 1].pai1 = pai1; // pai 1 do filho j+1
        popNova.indiv[j + 1].pai2 = pai2; // pai 2 do filho j+1

        j = j + 2; // incremento do  ndice do individuo

    } while (j < tamPop);

}