//
// Created by Daniel on 19/09/2025.
//

#include "Parameters.h"

class ESN;
// Iniciando os Parametros da ESN
int input_size = 6;
int reservoir_size = 25;
int n_out = 4;
double spectral_radius_d = 0.95;
double sparsity = 0.1;
int n_stab = 10;
int n_examples = 10;

// Iniciando os Parametros do AG
int tamPop = 100;
int lcrom = (reservoir_size + 1) * n_out;
populacao popVelha, popNova;
int nroMaxExec = 30;//30 // numero de repeticoes do AG
int maxGen = 500;//500    // numero maximo de geracoes
bool elitismo = true; // true: elitismo ; false: sem elitismo
int tamTorneio = 3;   // tamanho do torneio (usado na selecao por torneio)
double taxaCross = 0.6;       // taxa de crossover
double taxaMut = 1.0 / lcrom; // taxa de mutacao

// Iniciando os parametros do DE
double CR_DE=0.9; 									    // crossover probability: DE parameter CR
int crossover_type = 3;								    // crossover type: 1 - Binomial Crossover; 2 - VIntX; 3 - GbinX
int mutation_type = 2;									// mutation type: 1 - DE/rand/1; 2 - DE/best/1
double F_DE=0.5;									    // differential weight: DE parameter F
double pLL=0.2;										    // probability of testing LinkageLearning in an offspring
int gen;											    // generation

// Iniciando variaveis globais
Random *random2;
ESN *esn;
double *arq_media_fitness, *arq_melhor_fitness;     // dados para serem gravados
double **arq_melhor_individuo;                      // dados para serem gravados
int stop_cr=1;										// stop_criterion: 0-time; 1-nfeval
int stop_flag;										// stop flag
long int max_nfeval = 50100;					    // maximum nfeval for experiments with maximum number of fitness evaluations
long int nfeval;									// number of fitness evaluations
double *bestX;
double *bestY;
double *pX;
double *pY;
double bestFitness;
double *bestAcoes;
int *file_gen;										// data to be stored: number of generations
int *file_nfeval;									// data to be stored: number of fitness evaluations
int *vsort_aux;										// auxiliar sorted vector of integers (used in different methods)
double *file_best_fitness, *time_run;				// data to be stored: best fitness, runtime
double *file_best_fitness_002, *file_best_fitness_020, *file_best_fitness_040 ;				// data to be stored: best fitness at different times
double *file_best_fitness_gen;						// data to be stored: best fitness over the generations for run 0
double *file_n_edges_eVIG;							// data to be stored: number of edges of the eVIG
double *file_n_edges_eVIG_gen;						// data to be stored: number of edges of the eVIG over the generations for run 0
double **File_best_ind;								// data to be stored: best individual
int save_datagen_flag=1;
int LL_flag = 1;									// linkage learning flag (0-without LinkageLearning; 1-with online LinkageLearning; 2-with offline LinkageLearning)
double *weightVert_m;								// Mean variable importance vector
double **Madj_m;									// Mean Adjacency Matrix (2-variable interaction)
clock_t time_start;									// starting time
double max_time;									// maximum time for experiments with maximum time
int flag_best_fitness_002, flag_best_fitness_020, flag_best_fitness_040;				// data to be stored: flag to save best fitness at different times

// Simulador
int nsteps = 300;
int st_pos;
int nForward; // numero de vezes que andou pra frente
int nRot45;
int nRotInv45;
int nRot90;
int gDireto;
int gProbMem;
int pBeta;

// Variaveis para salvar dados da ESN
double **reservoir_activations_steps = nullptr;
double **output_activations_steps = nullptr;
double **best_reservoir_activations_steps = nullptr;
double **best_output_activations_steps = nullptr;
double *reservoir_activations = nullptr;
double *output_activations = nullptr;