//
// Created by Daniel on 24/09/2025.
//

#include "../../parameters/Parameters.h"

void statistics(populacao *pop, const int n_run, const int n_edges_eVIG) {
    if (stop_flag == 0) {
        pop->somaFitness = pop->indiv[0].fitness; // sum of the fitness in the population
        pop->maxFitness = pop->indiv[0].fitness; // maximum fitness in the population
        pop->melhorIndividuo = 0; // best individual in the population
        if (pop->indiv[0].fitness > file_best_fitness[n_run]) {
            file_best_fitness[n_run] = pop->indiv[0].fitness;
            for (int gene = 0; gene < lcrom; gene++)
                File_best_ind[n_run][gene] = pop->indiv[0].cromossomo[gene];
        }
        for (int j = 1; j < tamPop; j++) {
            pop->somaFitness = pop->somaFitness + pop->indiv[j].fitness;
            if (pop->indiv[j].fitness > pop->maxFitness) {
                pop->maxFitness = pop->indiv[j].fitness;
                pop->melhorIndividuo = j;
            }
            if (pop->indiv[j].fitness > file_best_fitness[n_run]) {
                file_best_fitness[n_run] = pop->indiv[j].fitness;
                for (int gene = 0; gene < lcrom; gene++)
                    File_best_ind[n_run][gene] = pop->indiv[j].cromossomo[gene];
            }
        }

        pop->mediaFitness = pop->somaFitness / tamPop; // mean fitness in the population

        // Save data for fitness along the generations: only for the first run
        if (save_datagen_flag == 1 && n_run == 0) {
            if (gen < maxGen) {
                file_best_fitness_gen[gen] = pop->maxFitness;
                // for LL
                // Record number of edges of the eVIG along the generations
                file_n_edges_eVIG_gen[gen] = n_edges_eVIG;
            }
        }

        // Save fitness at diffferent times
        if (flag_best_fitness_002 == 0 && nfeval >= 0.02 * max_nfeval) {
            flag_best_fitness_002 = 1;
            file_best_fitness_002[n_run] = file_best_fitness[n_run];
        }
        if (flag_best_fitness_020 == 0 && nfeval >= 0.20 * max_nfeval) {
            flag_best_fitness_020 = 1;
            file_best_fitness_020[n_run] = file_best_fitness[n_run];
        }
        if (flag_best_fitness_040 == 0 && nfeval >= 0.40 * max_nfeval) {
            flag_best_fitness_040 = 1;
            file_best_fitness_040[n_run] = file_best_fitness[n_run];
        }
    }
}
