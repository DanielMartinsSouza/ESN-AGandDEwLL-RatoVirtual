//
// Created by Daniel on 24/09/2025.
//

#include "../../parameters/Parameters.h"
#include "../../core/estVIG2.h"

void mutation_DErand1(alelo* offspring, const int j)
{
    // Selection of the Parents
    int a = j;
    while (a == j)
        a = random_int(0, tamPop - 1);
    int b = j;
    while (b == j || b == a)
        b = random_int(0, tamPop - 1);
    int c = j;
    while (c == j || c == a || c == b)
        c = random_int(0, tamPop - 1);

    // Transformation
    for (int gene = 0; gene < lcrom; gene++)
    {
        offspring[gene] = popVelha.indiv[a].cromossomo[gene] + F_DE * (popVelha.indiv[b].cromossomo[gene] - popVelha.
            indiv[c].cromossomo[gene]);
    }
}

void mutation_DEbest1(alelo* offspring, const int j)
{
    // Selection of the Parents
    const int a = popVelha.melhorIndividuo;
    int b = j;
    while (b == j || b == a)
        b = random_int(0, tamPop - 1);
    int c = j;
    while (c == j || c == a || c == b)
        c = random_int(0, tamPop - 1);

    // Transformation
    for (int gene = 0; gene < lcrom; gene++)
        offspring[gene] = popVelha.indiv[a].cromossomo[gene] + F_DE * (popVelha.indiv[b].cromossomo[gene] - popVelha.
            indiv[c].cromossomo[gene]);
}

void mutationDE(alelo* offspring, const int j)
{
    if (mutation_type == 1)
        mutation_DErand1(offspring, j);
    else if (mutation_type == 2)
        mutation_DEbest1(offspring, j);
}

void UX_DE(const alelo* parent1, const alelo* parent2, alelo* offspring)
{
    const int jrand = random_int(0, lcrom - 1);
    for (int gene = 0; gene < lcrom; gene++)
    {
        if (const double aux = random_dou(); aux < CR_DE || gene == jrand)
            offspring[gene] = parent1[gene];
        else
            offspring[gene] = parent2[gene];
    }
}

void crossoverDE(const alelo* parent1, const alelo* parent2, alelo* offspring, estVIG2* eVIG_instance, double pdyn)
{
    if (crossover_type == 1)
    {
        UX_DE(parent1, parent2, offspring); // Binomial Crossover
    }
    else if (crossover_type == 2)
    {
        if (random_dou() < pdyn)
        {
            UX_DE(parent1, parent2, offspring); // Binomial Crossover
        }
        else
        {
            eVIG_instance->VIntCrossover_DE(parent1, parent2, offspring); // Variable Interaction Crossover (VIntX)
        }
    }
    else if (crossover_type == 3)
    {
        if (random_dou() < pdyn)
        {
            UX_DE(parent1, parent2, offspring); // Binomial Crossover
        }
        else
        {
            eVIG_instance->GbinX(parent1, parent2, offspring); // Group-wise Binomial Crossover (GbinX)
        }
    }
}
