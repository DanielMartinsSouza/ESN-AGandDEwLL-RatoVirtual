//
// Created by Daniel on 19/09/2025.
//
#include <iostream>
#include "../parameters/Parameters.h"
#include <fstream>

using namespace std;
constexpr char nameDir[] = "output";

void arq_saida(const int nroExec, const int seed)
{
    // mkdir(nameDir);

    int i;
    FILE *Fit_arq, *Melfit_arq;
    char nome[64];

    const char *nome_p = nome;

    // Media do Fitness da Populacao
    sprintf(nome, "%s/fit_%d.dat", nameDir, nroExec);
    if ((Fit_arq = fopen(nome_p, "at+")) == nullptr)
    {
        cout << "O arquivo de gravacao dos dados de Media do Fitness nao pode ser aberto " << endl;
        exit(1);
    }
    for (i = 1; i <= maxGen; i++)
        fprintf(Fit_arq, "%.3lf\n", arq_media_fitness[i]);

    fclose(Fit_arq);

    // Fitness do Melhor Individuo de Cada Geracao
    sprintf(nome, "%s/mfi_%d.dat", nameDir, nroExec);
    if ((Melfit_arq = fopen(nome_p, "at+")) == nullptr)
    {
        cout << "O arquivo de gravacao dos dados de Melhor Fitness nao pode ser aberto " << endl;
        exit(1);
    }
    for (i = 1; i <= maxGen; i++)
        fprintf(Melfit_arq, "%.3lf\n", arq_melhor_fitness[i]);

    fclose(Melfit_arq);

    // Melhor Individuo de Cada Geracao

    sprintf(nome, "%s/MelhoresIndividuos_%d.dat", nameDir, nroExec);
    FILE *arq = fopen(nome, "at+");

    if (arq != nullptr)
    {
        for (i = 1; i <= maxGen; i++)
        {
            for (int gene = 0; gene < lcrom; gene++)
                fprintf(arq, "%lf ", arq_melhor_individuo[i][gene]);
            fprintf(arq, "\n");
        }
        if (int closeResult = fclose(arq); closeResult == 0)
            cout << "\nMelhores Individuos salvos com sucesso!" << endl;
        else
            cout << "\nErro ao fechar o arquivo!" << endl;
    }
    else
        cout << "\nErro ao abrir o arquivo!" << endl;

    char filename[20];
            sprintf(filename, "output/seed_%d.txt", nroExec);
            ofstream ofs3;
            ofs3.open(filename, ofstream::out);
            ofs3 << seed << endl;
            ofs3.close();

            // salvar numero de vezes que andou pra frente do melhor fitness
            char name[20];
            sprintf(name, "output/nForward_%d.txt", nroExec);
            ofstream ofs;
            ofs.open(name, ofstream::out);
            ofs << "nRot45;nRotInv45;nRot90;nForward;gDireto;gProbMem;pBeta;nsteps" << endl;
            ofs << nRot45 << ";" << nRotInv45 << ";" << nRot90 << ";" << nForward << ";"
                    << gDireto << ";" << gProbMem << ";" << pBeta << ";" << nsteps << endl;
            ofs.close();

            // salvar posicoes do melhor fitness
            char name2[20];
            sprintf(name2, "output/position_%d.txt", nroExec);
            ofstream ofs2;
            ofs2.open(name2, ofstream::out);
            for (int j = 0; j <= nsteps; j++) {
                ofs2 << j << ";" << bestX[j] << ";" << bestY[j] << endl;
            }
            ofs2.close();

            // salvar dados da ESN
            // Salvar ativa  es de todos os passos do melhor fitness
            char activationsStepsFile[30];
            sprintf(activationsStepsFile, "output/activations_steps_%d.txt", nroExec);
            ofstream ofsSteps;
            ofsSteps.open(activationsStepsFile, ofstream::out);

            if (ofsSteps.is_open()) {
                ofsSteps << "Passo;Reservat rio (Ativa  es);Sa da (Ativa  es)" << endl;
                for (int step = 0; step < nsteps; step++) {
                    ofsSteps << step << ";";
                    for (int k = 0; k < reservoir_size; k++) {
                        ofsSteps << best_reservoir_activations_steps[step][k];
                        if (k < reservoir_size - 1)
                            ofsSteps << ",";
                    }
                    ofsSteps << ";";
                    for (int l = 0; l < n_out; l++) {
                        ofsSteps << best_output_activations_steps[step][l];
                        if (l < n_out - 1)
                            ofsSteps << ",";
                    }
                    ofsSteps << endl;
                }
                ofsSteps.close();
            } else {
                cerr << "Erro ao abrir arquivo para salvar ativa  es por passos: " << activationsStepsFile << endl;
            }

            // salvar a  es do melhor fitness
            char actionsFile[30];
            sprintf(actionsFile, "output/actions_%d.txt", nroExec);
            ofstream ofsActions;
            ofsActions.open(actionsFile, ofstream::out);

            if (ofsActions.is_open()) {
                ofsActions << "Passo;A  o" << endl;
                for (int step = 0; step <= nsteps; step++) {
                    ofsActions << step << ";" << bestAcoes[step] << endl;
                }
                ofsActions.close();
            } else {
                cerr << "Erro ao abrir arquivo para salvar a  es: " << actionsFile << endl;
            }

    salv_pop(nroExec);
}

void salv_pop(int nroExec)
{
    char nome[64];

    sprintf(nome, "%s/pop_%d.dat", nameDir, nroExec);
    FILE *Pop_arq = fopen(nome, "wt");

    if (Pop_arq != nullptr)
    {
        // Salva popula  o
        for (int i = 0; i < tamPop; i++)
        {
            for (int gene = 0; gene < lcrom; gene++)
                fprintf(Pop_arq, "%lf ", popVelha.indiv[i].cromossomo[gene]);
            fprintf(Pop_arq, "%lf ", popVelha.indiv[i].fitness);
            fprintf(Pop_arq, "\n");
        }
        fprintf(Pop_arq, "%lf ", popVelha.somaFitness);		// soma fitness
        fprintf(Pop_arq, "%lf ", popVelha.maxFitness);		// maior fitness
        fprintf(Pop_arq, "%d ", popVelha.melhorIndividuo);	// melhor individuo
        fprintf(Pop_arq, "%d ", popVelha.melhorIndividuo2); // melhor2 individuo
        fprintf(Pop_arq, "%lf ", popVelha.mediaFitness);	// media fitness

        int closeResult = fclose(Pop_arq);
        if (closeResult == 0)
            cout << "Populacao salva com sucesso!" << endl;
        else
            cout << "\nErro ao fechar o arquivo pop.dat!" << endl;
    }
    else
        cout << "\nErro ao abrir o arquivo pop.dat!" << endl;
}

void apaga_arquivos(const int nroExec)
{
    char nome[64];

    sprintf(nome, "%s/pop_%d.dat", nameDir, nroExec);
    remove(nome);
    sprintf(nome, "%s/MelhoresIndividuos_%d.dat", nameDir, nroExec);
    remove(nome);
    sprintf(nome, "%s/fit_%d.dat", nameDir, nroExec);
    remove(nome);
    sprintf(nome, "%s/mfi_%d.dat", nameDir, nroExec);
    remove(nome);
    cout << "Arquivos apagados com sucesso!" << endl
         << endl;
}