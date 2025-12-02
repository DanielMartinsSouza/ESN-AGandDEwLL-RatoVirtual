//
// Created by Daniel on 19/09/2025.
//
#include "../../parameters/Parameters.h"

int selecaoElitismo(const populacao* pop, int j)
{
    int individuoEscolhido;

    if (j == 0)
    {
        individuoEscolhido = pop->melhorIndividuo;
    }
    else
    {
        individuoEscolhido = pop->melhorIndividuo2;
    }
    return individuoEscolhido;
}

int selecaoTorneio(const populacao* pop)
{
    int individuo_esc = random_int(0, (tamPop - 1));
    for (int i = 1; i < tamTorneio; i++)
    {
        if (const int individuo_rand = random_int(0, (tamPop - 1)); pop->indiv[individuo_rand].fitness > pop->indiv[
            individuo_esc].fitness)
            individuo_esc = individuo_rand;
    }
    return individuo_esc;
}

int selecao(const populacao* pop, int j)
{
    int individuoEscolhido;

    if (elitismo == 1 && j < 2)
    {
        individuoEscolhido = selecaoElitismo(pop, j);
    }
    else
    {
        individuoEscolhido = selecaoTorneio(pop);
    }

    return individuoEscolhido;
}
