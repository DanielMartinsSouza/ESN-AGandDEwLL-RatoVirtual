//
// Created by Daniel on 19/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H
#define CHAR_LEN 1000
#define EPS2 1.0e-8 // used to compare solution elements
#define EPS4 1.0e-3 // used to compare fitness
#include <ctime>
#include "../core/estVIG2.h"
#include "../core/Random.h"
#include "../core/ESN.h"
/* Definicao dos tipos de dados */
typedef double alelo; // tipo de dado que os alelos podem assumir
typedef struct estrutura_individuo {
    alelo *cromossomo; // cromossomo
    double fitness; // funcao de avaliacao
    int pai1, pai2; // indice dos pais
} individuo;

typedef struct {
    individuo *indiv;
    double somaFitness;
    double mediaFitness;
    double maxFitness;
    int melhorIndividuo;
    int melhorIndividuo2;
} populacao;

// utils_functions
double *aloc_vectord(int lines);

individuo *aloc_vectorind(int lines);

double **aloc_matrixd(int lines, int collums);

void desaloc_matrixd(double **Matrix, int lines);

double random_dou();

int random_int(int L_range, int H_range);

double normEuc(const double *x, int l);

void multMatrixVect(double *y, double **A, int l_A, int c_A, const double *x, int l_x);

double largEig(double **M, int l, int c);

int *aloc_vectori(int lines);

int **aloc_matrixi(int lines, int collums);

void desaloc_matrixi(int **Matrix, int lines);

void rand_perm_size(const int *inp, int *out, int size_inp, int size_out);

// ESN variaveis globais
extern int input_size, reservoir_size, n_out, n_stab, n_examples;
extern double spectral_radius_d, sparsity;

// AG Variaveis Globais
extern int tamPop, lcrom, maxGen;
extern populacao popVelha, popNova;
extern double *arq_media_fitness, *arq_melhor_fitness; // dados para serem gravados
extern double **arq_melhor_individuo; // dados para serem gravados
extern int nroMaxExec;
extern bool elitismo;
extern int tamTorneio;
extern double taxaCross, taxaMut; // taxas de crossover e mutacao

// DE Variaveis Globais
extern double CR_DE; // crossover probability: DE parameter CR
extern int crossover_type; // crossover type: 1 - Binomial Crossover; 2 - VIntX; 3 - GbinX
extern int mutation_type; // mutation type: 1 - DE/rand/1; 2 - DE/best/1
extern double F_DE; // differential weight: DE parameter F
extern double pLL;
extern int gen;
extern double *file_best_fitness, *time_run; // data to be stored: best fitness, runtime
extern double *file_best_fitness_002, *file_best_fitness_020, *file_best_fitness_040;
// data to be stored: best fitness at different times
extern double *file_best_fitness_gen; // data to be stored: best fitness over the generations for run 0
extern double *file_n_edges_eVIG; // data to be stored: number of edges of the eVIG
extern double *file_n_edges_eVIG_gen; // data to be stored: number of generations
extern double **File_best_ind; // data to be stored: best individual
extern int *file_gen; // data to be stored: number of generations
extern int *file_nfeval;
extern int *vsort_aux;
// data to be stored: number of fitness evaluations                                                    // auxiliar sorted vector of integers (used in different methods)
extern int save_datagen_flag; // flag for saving data for generation in the first run
extern int LL_flag;
// linkage learning flag (0-without LinkageLearning; 1-with online LinkageLearning; 2-with offline LinkageLearning)
extern double *weightVert_m; // Mean variable importance vector
extern double **Madj_m; // Mean Adjacency Matrix (2-variable interaction)
extern clock_t time_start; // starting time
extern double max_time; // maximum time for experiments with maximum time
extern int flag_best_fitness_002, flag_best_fitness_020, flag_best_fitness_040;
// data to be stored: flag to save the best fitness at different times

// AG metodos
int main_AG();

void algGen(int nroExec, int seed);

void inicializacao(int nroExec);

void geracao();

void impressao(const populacao *pop, int gen_impressao);

double calcFitness(const alelo *indiv);

void impressaoAG(populacao *pop, int gen);

void estatistica(populacao *pop, int genAtual);

int selecao(const populacao *pop, int j);

void crossover(const alelo *pai1, const alelo *pai2, alelo *filho1, alelo *filho2, int j);

// DE metodos
int main_DE();

void de(int n_run);

void statistics(populacao *pop, int n_run, int n_edges_eVIG);

void mutationDE(alelo *offspring, int j);

void crossoverDE(const alelo *parent1, const alelo *parent2, alelo *offspring, estVIG2 *eVIG_instance, double pdyn);

void UX_DE(const alelo *parent1, const alelo *parent2, alelo *offspring);

double LL(alelo *cromossomo1, const alelo *cromossomo2, estVIG2 *eVIG_instance, double fitness);

// Variaveis globais
extern Random *random2;
extern ESN *esn;
extern int stop_cr; // stop_criterion: 0-time; 1-nfeval
extern int stop_flag; // stop flag
extern long int nfeval; // number of fitness evaluations
extern long int max_nfeval; // maximum nfeval for experiments with maximum number of fitness evaluations

// Simulação
extern int st_pos, nsteps, nForward, nRot45, nRot90, nRotInv45, gDireto, gProbMem, pBeta;
extern double *bestX, *bestY, *pX, *pY, bestFitness, *bestAcoes;
extern bool act;

// Armazenamento das saidas do reservatorios
extern double **reservoir_activations_steps; // Ativações do reservatório
extern double **output_activations_steps; // Ativações da saída
extern double **best_reservoir_activations_steps; // Melhor ativação do reservatório por passos
extern double **best_output_activations_steps; // Melhor ativação da saída por passos
extern double *reservoir_activations; // Ativações do reservatório (vetor)
extern double *output_activations; // Ativações da saída (vetor)

// Funcoes para salvar arquivos
void arq_saida(int nroExec, int seed);

void salv_pop(int nroExec);

void apaga_arquivos(int nroExec);

void file_output(int total_runs);

void salv_simulacao(int nroExec);

void salv_simulacaoDE(int nroExec);

#endif //ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H
