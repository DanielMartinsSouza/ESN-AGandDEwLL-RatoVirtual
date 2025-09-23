//
// Created by Daniel on 19/09/2025.
//
#include "../../parameters/Parameters.h"

double mutacao(const alelo valorAlelo)
{
    if (random2->nextFloat() < taxaMut)
        return valorAlelo + random2->nextGaussian(0, 1);
    return valorAlelo;
}

void crossover(const alelo *pai1, const alelo *pai2, alelo *filho1, alelo *filho2, const int j)
{
    int cruzamento1 = 0, cruzamento2 = 0;

    if (random_dou() < taxaCross)
    {
        while (cruzamento1 == cruzamento2)
        {
            cruzamento1 = random_int(0, lcrom - 1); // define ponto de cruzamento entre 1 e l-1
            cruzamento2 = random_int(0, lcrom - 1); // define ponto de cruzamento entre 1 e l-1
        }

        if (cruzamento1 > cruzamento2)
        {
            const int auxiliar = cruzamento1;
            cruzamento1 = cruzamento2;
            cruzamento2 = auxiliar;
        }
    }
    if (elitismo == 1 && j < 2)
    {
        cruzamento1 = 0;
        cruzamento2 = 0;
    }

    for (int gene = 0; gene < lcrom; gene++)
    {
        if (gene < cruzamento1 || gene >= cruzamento2)
        {
            if (elitismo == 1 && j < 2)
            {
                filho1[gene] = pai1[gene]; // filho 1
                filho2[gene] = pai2[gene];
            }
            else
            {
                filho1[gene] = mutacao(pai1[gene]); // filho 1
                filho2[gene] = mutacao(pai2[gene]);
            }
        }
        else
        {
            filho1[gene] = mutacao(pai2[gene]); // filho 1
            filho2[gene] = mutacao(pai1[gene]);
        }
    }
}