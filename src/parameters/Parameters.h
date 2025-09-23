//
// Created by Daniel on 19/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H

#include "../core/Random.h"
#include "../core/ESN.h"
/* Definicao dos tipos de dados */
typedef double alelo; // tipo de dado que os alelos podem assumir
typedef struct estrutura_individuo
{
    alelo *cromossomo; // cromossomo
    double fitness;	   // funcao de avaliacao
    int pai1, pai2;	   // indice dos pais
} individuo;

typedef struct
{
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

// ESN variaveis globais
extern int input_size, reservoir_size, n_out, n_stab, n_examples;
extern double spectral_radius_d, sparsity;

// AG Variaveis Globais
extern int tamPop, lcrom, maxGen;
extern populacao popVelha, popNova;
extern double *arq_media_fitness, *arq_melhor_fitness;                          // dados para serem gravados
extern double **arq_melhor_individuo;				                            // dados para serem gravados
extern int nroMaxExec;
extern bool elitismo;
extern int tamTorneio;
extern double taxaCross, taxaMut;					                            // taxas de crossover e mutacao

// AG metodos
int main_AG();
void algGen(int nroExec, int seed);
void inicializacao(int nroExec);
void geracao();
void impressao(const populacao *pop, int gen);
double calcFitness(const alelo *indiv);
void impressaoAG(populacao *pop, int gen);
void estatistica(populacao *pop, int gen);
int selecao(const populacao *pop, int j);
void crossover(const alelo *pai1, const alelo *pai2, alelo *filho1, alelo *filho2, int j);

// Variaveis globais
extern Random *random2;
extern ESN *esn;
extern int stop_cr;																// stop_criterion: 0-time; 1-nfeval
extern int stop_flag;															// stop flag
extern long int nfeval;															// number of fitness evaluations
extern long int max_nfeval;														// maximum nfeval for experiments with maximum number of fitness evaluations

// Simulação
extern int st_pos, nsteps, nForward, nRot45, nRot90, nRotInv45, gDireto, gProbMem, pBeta;
extern double *bestX, *bestY, *pX, *pY, bestFitness, *bestAcoes;
extern bool act;

// Armazenamento das saidas do reservatorios
extern double **reservoir_activations_steps;	                                // Ativações do reservatório
extern double **output_activations_steps;		                                // Ativações da saída
extern double **best_reservoir_activations_steps;                               // Melhor ativação do reservatório por passos
extern double **best_output_activations_steps;	                                // Melhor ativação da saída por passos
extern double *reservoir_activations;			                                // Ativações do reservatório (vetor)
extern double *output_activations;				                                // Ativações da saída (vetor)

// Funcoes para salvar arquivos
void arq_saida(int nroExec, int seed);
void salv_pop(int nroExec);
void apaga_arquivos(int nroExec);

#endif //ESN_AGANDDEWLL_RATOVIRTUAL_DEFPARAMETERS_H