//
// Created by Daniel on 19/09/2025.
//

#include "Parameters.h"

class ESN;
// Iniciando os Parametros da ESN
int input_size = 8;
int reservoir_size = 50;
int n_out = 4;
double spectral_radius_d = 0.95;
double sparsity = 0.1;
int n_stab = 10;
int n_examples = 10;

// Iniciando os Parametros do AG
int tamPop = 100;
int lcrom = (reservoir_size + 1) * n_out;
populacao popVelha, popNova;
int nroMaxExec = 30; // numero de repeticoes do AG
int maxGen = 500;    // numero maximo de geracoes
bool elitismo = true; // true: elitismo ; false: sem elitismo
int tamTorneio = 3;   // tamanho do torneio (usado na selecao por torneio)
double taxaCross = 0.6;       // taxa de crossover
double taxaMut = 1.0 / lcrom; // taxa de mutacao

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
bool act;

// Variaveis para salvar dados da ESN
double **reservoir_activations_steps = nullptr;
double **output_activations_steps = nullptr;
double **best_reservoir_activations_steps = nullptr;
double **best_output_activations_steps = nullptr;
double *reservoir_activations = nullptr;
double *output_activations = nullptr;