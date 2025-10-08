//
// Created by Daniel on 23/09/2025.
//
#include <cmath>
#include <ctime>
#include <fstream>
#include "../../parameters/Parameters.h"
#include <iostream>
#include "../../core/estVIG2.h"


using namespace std;

int main_DE() {
    // Alocação de vetores e matrizes
    File_best_ind = aloc_matrixd(nroMaxExec, lcrom);
    file_best_fitness_gen = aloc_vectord(maxGen);
    file_best_fitness = aloc_vectord(nroMaxExec);
    file_best_fitness_002 = aloc_vectord(nroMaxExec);
    file_best_fitness_020 = aloc_vectord(nroMaxExec);
    file_best_fitness_040 = aloc_vectord(nroMaxExec);
    file_n_edges_eVIG = aloc_vectord(nroMaxExec);
    file_n_edges_eVIG_gen = aloc_vectord(maxGen);
    file_gen = aloc_vectori(nroMaxExec);
    file_nfeval = aloc_vectori(nroMaxExec);
    time_run = aloc_vectord(nroMaxExec);
    vsort_aux = aloc_vectori(lcrom); // Auxiliar sorted vector of integers (used in different methods)

    for (int i = 0; i < lcrom; i++)
        vsort_aux[i] = i;
    if (save_datagen_flag == 1 && LL_flag > 0) {
        weightVert_m = aloc_vectord(lcrom);
        Madj_m = aloc_matrixd(lcrom, lcrom);
        for (int i = 0; i < lcrom; i++)
            for (int j = 0; j < lcrom; j++)
                Madj_m[i][j] = 0.0;
    }

    cout << "\n ***** Differential Evolution ****" << endl;

    pX = new double[nsteps + 1];
    pY = new double[nsteps + 1];
    bestX = new double[nsteps + 1];
    bestY = new double[nsteps + 1];
    bestAcoes = new double[nsteps + 1];

    const int seed = time(NULL);
    // ESN(int reservoir_size, double sparsity, double spectral_radius_d, int n_out, int input_size, int n_examples, int n_stab, int seed)
    esn = new ESN(reservoir_size, sparsity, spectral_radius_d, n_out, input_size, n_examples, n_stab, seed);

    for (int n_run = 0; n_run < nroMaxExec; n_run++) {

        /* Teste */
        bestFitness = numeric_limits<double>::min(); // menor double negativo
        random2 = new Random(1, n_run + 1); // semente para gerar os numeros aleatorios

        cout << "Run:" << n_run << ", random seed: " << n_run + 1 << endl;
        srand(n_run + 1); // random seed  (for run)
        de(n_run); // run DE
        salv_simulacaoDE(n_run);
    }

    file_output(nroMaxExec); // save data

    // Desallocation of vectors and matrices
    desaloc_matrixd(File_best_ind, nroMaxExec);
    if (save_datagen_flag == 1 && LL_flag > 0) {
        desaloc_matrixd(Madj_m, lcrom);
        delete[] weightVert_m;
    }

    // Deleting population
    for (int num_ind = 0; num_ind < tamPop; num_ind++) {
        delete[] popVelha.indiv[num_ind].cromossomo;
        delete[] popNova.indiv[num_ind].cromossomo;
    }
    delete[] popVelha.indiv;
    delete[] popNova.indiv;
    delete[] pX;
    delete[] pY;
    delete[] bestX;
    delete[] bestY;
    delete[] bestAcoes;
    delete[] time_run;
    delete[] file_gen;
    delete[] file_nfeval;
    delete[] file_best_fitness;
    delete[] file_best_fitness_002;
    delete[] file_best_fitness_020;
    delete[] file_best_fitness_040;
    delete[] file_best_fitness_gen;
    delete[] file_n_edges_eVIG;
    delete[] file_n_edges_eVIG_gen;
    delete[] vsort_aux;
    delete esn;

    return 0;
}

void inicializacaoDE(const int num_ind) {
    for (int gene = 0; gene < lcrom; gene++) {
        popVelha.indiv[num_ind].cromossomo[gene] = random2->nextFloat(-1, 1);
    }
    popVelha.indiv[num_ind].fitness = calcFitness(popVelha.indiv[num_ind].cromossomo);
}

void print_data(const populacao * pop_velha) {
    cout << "Geracao: " << gen << endl;
    cout << "Individuo com melhor fitness: " << pop_velha->melhorIndividuo << endl;
    cout << "Fitness do melhor Individuo: " << pop_velha->maxFitness << endl;
    cout << "Media do Fitness da geracao: " << pop_velha->mediaFitness << endl
         << endl
         << endl;

}

void initiatePop(const estVIG2 * est_vig2, const int n_run) {
    // Dynamic allocation: populations
    popVelha.indiv = aloc_vectorind(tamPop);
    popNova.indiv = aloc_vectorind(tamPop);

    for (int num_ind = 0; num_ind < tamPop; num_ind++) {
        // Dynamic allocation: chromosomes
        popVelha.indiv[num_ind].cromossomo = aloc_vectord(lcrom);
        popNova.indiv[num_ind].cromossomo = aloc_vectord(lcrom);

        // Random Initialization
        //randomInd(num_ind); // generate random individual
        inicializacaoDE(num_ind);
    }
    flag_best_fitness_002 = 0;
    flag_best_fitness_020 = 0;
    flag_best_fitness_040 = 0;
    file_best_fitness[n_run] = popVelha.indiv[0].fitness;
    for (int i = 0; i < lcrom; i++)
        File_best_ind[n_run][i] = popVelha.indiv[0].cromossomo[i];
    statistics(&popVelha, n_run, est_vig2->n_edges_eVIG);
    print_data(&popVelha);
}

void generation_DE(estVIG2 * est_vig2) {
    int j = 0;
    individuo y, xnew;

    y.cromossomo = aloc_vectord(lcrom);
    xnew.cromossomo = aloc_vectord(lcrom);

    do {
        const double pdyn = exp(-2.0 * (nfeval - 1.0) / max_nfeval);
        // remember that there are initial fitness evaluations for the offline LinkageLearning
        // cout<<pdyn<<endl;

        // reproduction
        mutationDE(y.cromossomo, j);
        crossoverDE(y.cromossomo, popVelha.indiv[j].cromossomo, xnew.cromossomo, est_vig2, pdyn);

        // Evaluation of the offspring xnew
        xnew.fitness = calcFitness(xnew.cromossomo);

        // (online) Linkage Learning
        if (LL_flag == 1 && stop_flag == 0 && random_dou() < pLL * pdyn)
            xnew.fitness = LL(xnew.cromossomo, popVelha.indiv[j].cromossomo, est_vig2, xnew.fitness);
        // Selection for next population
        if (xnew.fitness > popVelha.indiv[j].fitness) {
            for (int gene = 0; gene < lcrom; gene++)
                popNova.indiv[j].cromossomo[gene] = xnew.cromossomo[gene];
            //popNova.indiv[j].fitness = xnew.fitness;
            popNova.indiv[j].fitness = calcFitness(popNova.indiv[j].cromossomo);
        } else {
            for (int gene = 0; gene < lcrom; gene++)
                popNova.indiv[j].cromossomo[gene] = popVelha.indiv[j].cromossomo[gene];
            //popNova.indiv[j].fitness = popVelha.indiv[j].fitness;
            popNova.indiv[j].fitness = calcFitness(popNova.indiv[j].cromossomo);
        }

        j = j + 1;
    } while (j < tamPop && stop_flag == 0);

    delete[] y.cromossomo;
    delete[] xnew.cromossomo;
}

void copy_pop() {
    for (int ind = 0; ind < tamPop; ind++) {
        popVelha.indiv[ind].fitness = popNova.indiv[ind].fitness;
        for (int gene = 0; gene < lcrom; gene++)
            popVelha.indiv[ind].cromossomo[gene] = popNova.indiv[ind].cromossomo[gene];
    }
}

void de(const int n_run) {
    // Initialization
    time_start = clock();
    stop_flag = 0;
    max_time = lcrom / 2.0; // used only if stop criterion is time
    nfeval = 0; // number of fitness evaluations
    gen = 0; // generation
    auto *eVIG_instance = new estVIG2(lcrom, CR_DE, LL_flag, crossover_type); // from class estVIG2 (estVIG2.h)

    initiatePop(eVIG_instance, n_run); // initiating population

    // Iterations of the Optimizer
    do {
        gen++; // generation index

        generation_DE(eVIG_instance); // generation procedure

        copy_pop();
        // popold=popnew
        statistics(&popVelha, n_run, eVIG_instance->n_edges_eVIG);

        print_data(&popVelha);

    } while (stop_flag == 0);

    // Data to be saved
    time_run[n_run] = ((double) (clock() - time_start)) / ((double) CLOCKS_PER_SEC);
    file_gen[n_run] = gen;
    file_nfeval[n_run] = nfeval;
    // for LinkageLearning
    file_n_edges_eVIG[n_run] = eVIG_instance->n_edges_eVIG;
    // eVIG_instance->print();
    if (save_datagen_flag == 1 && LL_flag > 0) {
        for (int i = 0; i < lcrom; i++) {
            weightVert_m[i] += eVIG_instance->weightVert[i];
            if (n_run == nroMaxExec - 1) {
                weightVert_m[i] /= nroMaxExec;
            }
            for (int j = 0; j < lcrom; j++) {
                Madj_m[i][j] += eVIG_instance->Madj[i][j];
                if (n_run == nroMaxExec - 1) {
                    Madj_m[i][j] /= nroMaxExec;
                }
            }
        }
        if (n_run == nroMaxExec - 1) {
            // Save eVIG
            eVIG_instance->save(Madj_m, weightVert_m, LL_flag, mutation_type, crossover_type);
        }
    }
    delete eVIG_instance;
}
