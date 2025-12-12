//
// Created by Daniel on 19/09/2025.
//

#include "../../parameters/Parameters.h"

void estatistica(populacao* pop, const int genAtual)
{
    int j;

    pop->somaFitness = pop->indiv[0].fitness; // soma do fitness da populacao
    pop->maxFitness = pop->indiv[0].fitness; // maximo valor de fitness da populacao
    pop->melhorIndividuo = 0; // melhor individuo da populacao
    pop->melhorIndividuo2 = 0; // segundo melhor individuo da populacao

    for (j = 1; j < tamPop; j++)
    {
        pop->somaFitness = pop->somaFitness + pop->indiv[j].fitness;
        if (pop->indiv[j].fitness >= pop->maxFitness)
        {
            pop->maxFitness = pop->indiv[j].fitness;
            pop->melhorIndividuo2 = pop->melhorIndividuo;
            pop->melhorIndividuo = j;
        }
    }

    pop->mediaFitness = pop->somaFitness / tamPop;

    // Dados para serem salvos
    arq_media_fitness[genAtual] = pop->mediaFitness;
    arq_melhor_fitness[genAtual] = pop->maxFitness;

    const int n = pop->melhorIndividuo;
    for (j = 0; j < lcrom; j++){
        arq_melhor_individuo[genAtual][j] = pop->indiv[n].cromossomo[j];
    }
}
