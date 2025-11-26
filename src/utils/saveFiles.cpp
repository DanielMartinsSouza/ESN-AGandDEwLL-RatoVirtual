//
// Created by Daniel on 19/09/2025.
//
#include <iostream>
#include "../parameters/Parameters.h"
#include <fstream>

using namespace std;
constexpr char nameDir[] = "output";

void salv_simulacaoDE(const int nroExec)
{
    // salvar numero de vezes que andou pra frente do melhor fitness
    char name[50];
    sprintf(name, "data/nForward_%d.txt", nroExec);
    ofstream ofs;
    ofs.open(name, ofstream::out);
    ofs << "nRot45;nRotInv45;nRot90;nForward;gDireto;gProbMem;pBeta;nsteps" << endl;
    ofs << nRot45 << ";" << nRotInv45 << ";" << nRot90 << ";" << nForward << ";"
        << gDireto << ";" << gProbMem << ";" << pBeta << ";" << nsteps << endl;
    ofs.close();

    // salvar posicoes do melhor fitness
    char name2[50];
    sprintf(name2, "data/position_%d.txt", nroExec);
    ofstream ofs2;
    ofs2.open(name2, ofstream::out);
    for (int j = 0; j <= nsteps; j++)
    {
        ofs2 << j << ";" << bestX[j] << ";" << bestY[j] << endl;
    }
    ofs2.close();

    // salvar dados da ESN
    // Salvar ativa  es de todos os passos do melhor fitness
    char activationsStepsFile[30];
    sprintf(activationsStepsFile, "data/activations_steps_%d.txt", nroExec);
    ofstream ofsSteps;
    ofsSteps.open(activationsStepsFile, ofstream::out);

    if (ofsSteps.is_open())
    {
        ofsSteps << "Passo;Reservat rio (Ativa  es);Sa da (Ativa  es)" << endl;
        for (int step = 0; step < nsteps; step++)
        {
            ofsSteps << step << ";";
            for (int k = 0; k < reservoir_size; k++)
            {
                ofsSteps << best_reservoir_activations_steps[step][k];
                if (k < reservoir_size - 1)
                    ofsSteps << ",";
            }
            ofsSteps << ";";
            for (int l = 0; l < n_out; l++)
            {
                ofsSteps << best_output_activations_steps[step][l];
                if (l < n_out - 1)
                    ofsSteps << ",";
            }
            ofsSteps << endl;
        }
        ofsSteps.close();
    }
    else
    {
        cerr << "Erro ao abrir arquivo para salvar ativa  es por passos: " << activationsStepsFile << endl;
    }

    // salvar a  es do melhor fitness
    char actionsFile[30];
    sprintf(actionsFile, "data/actions_%d.txt", nroExec);
    ofstream ofsActions;
    ofsActions.open(actionsFile, ofstream::out);

    if (ofsActions.is_open())
    {
        ofsActions << "Passo;A  o" << endl;
        for (int step = 0; step <= nsteps; step++)
        {
            ofsActions << step << ";" << bestAcoes[step] << endl;
        }
        ofsActions.close();
    }
    else
    {
        cerr << "Erro ao abrir arquivo para salvar a  es: " << actionsFile << endl;
    }
}

void salv_simulacao(const int nroExec)
{
    // salvar numero de vezes que andou pra frente do melhor fitness
    char name[50];
    sprintf(name, "output/nForward_%d.txt", nroExec);
    ofstream ofs;
    ofs.open(name, ofstream::out);
    ofs << "nRot45;nRotInv45;nRot90;nForward;gDireto;gProbMem;pBeta;nsteps" << endl;
    ofs << nRot45 << ";" << nRotInv45 << ";" << nRot90 << ";" << nForward << ";"
        << gDireto << ";" << gProbMem << ";" << pBeta << ";" << nsteps << endl;
    ofs.close();

    // salvar posicoes do melhor fitness
    char name2[50];
    sprintf(name2, "output/position_%d.txt", nroExec);
    ofstream ofs2;
    ofs2.open(name2, ofstream::out);
    for (int j = 0; j <= nsteps; j++)
    {
        ofs2 << j << ";" << bestX[j] << ";" << bestY[j] << endl;
    }
    ofs2.close();

    // salvar dados da ESN
    // Salvar ativações de todos os passos do melhor fitness
    char activationsStepsFile[50];
    sprintf(activationsStepsFile, "output/activations_steps_%d.txt", nroExec);
    ofstream ofsSteps;
    ofsSteps.open(activationsStepsFile, ofstream::out);

    if (ofsSteps.is_open())
    {
        ofsSteps << "Passo;Reservat rio (Ativa  es);Sa da (Ativa  es)" << endl;
        for (int step = 0; step < nsteps; step++)
        {
            ofsSteps << step << ";";
            for (int k = 0; k < reservoir_size; k++)
            {
                ofsSteps << best_reservoir_activations_steps[step][k];
                if (k < reservoir_size - 1)
                    ofsSteps << ",";
            }
            ofsSteps << ";";
            for (int l = 0; l < n_out; l++)
            {
                ofsSteps << best_output_activations_steps[step][l];
                if (l < n_out - 1)
                    ofsSteps << ",";
            }
            ofsSteps << endl;
        }
        ofsSteps.close();
    }
    else
    {
        cerr << "Erro ao abrir arquivo para salvar ativa  es por passos: " << activationsStepsFile << endl;
    }

    // salvar ações do melhor fitness
    char actionsFile[30];
    sprintf(actionsFile, "output/actions_%d.txt", nroExec);
    ofstream ofsActions;
    ofsActions.open(actionsFile, ofstream::out);

    if (ofsActions.is_open())
    {
        ofsActions << "Passo;A  o" << endl;
        for (int step = 0; step <= nsteps; step++)
        {
            ofsActions << step << ";" << bestAcoes[step] << endl;
        }
        ofsActions.close();
    }
    else
    {
        cerr << "Erro ao abrir arquivo para salvar a  es: " << actionsFile << endl;
    }
}

void arq_saida(const int nroExec, const int seed)
{
    // mkdir(nameDir);

    int i;
    FILE *Fit_arq, *Melfit_arq;
    char nome[64];

    const char* nome_p = nome;

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
    FILE* arq = fopen(nome, "at+");

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

    salv_simulacao(nroExec);

    salv_pop(nroExec);
}

void salv_pop(const int nroExec)
{
    char nome[64];

    sprintf(nome, "%s/pop_%d.dat", nameDir, nroExec);

    if (FILE* Pop_arq = fopen(nome, "wt"); Pop_arq != nullptr)
    {
        // Salva popula  o
        for (int i = 0; i < tamPop; i++)
        {
            for (int gene = 0; gene < lcrom; gene++)
                fprintf(Pop_arq, "%lf ", popVelha.indiv[i].cromossomo[gene]);
            fprintf(Pop_arq, "%lf ", popVelha.indiv[i].fitness);
            fprintf(Pop_arq, "\n");
        }
        fprintf(Pop_arq, "%lf ", popVelha.somaFitness); // soma fitness
        fprintf(Pop_arq, "%lf ", popVelha.maxFitness); // maior fitness
        fprintf(Pop_arq, "%d ", popVelha.melhorIndividuo); // melhor individuo
        fprintf(Pop_arq, "%d ", popVelha.melhorIndividuo2); // melhor2 individuo
        fprintf(Pop_arq, "%lf ", popVelha.mediaFitness); // media fitness

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

void file_output(const int total_runs)
{
    char name[CHAR_LEN];
    FILE *Bestfit_file, *Time_file, *Gen_file;
    FILE *Bestfit002_file, *Bestfit020_file, *Bestfit040_file;

    const char* name_p = name;
    const int N = lcrom;
    // Best fitness in each generation for run 0
    if (save_datagen_flag == 1)
    {
        FILE* Bfg_file;
        sprintf(name, "data/bfg_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
        if ((Bfg_file = fopen(name_p, "w")) == nullptr)
        {
            puts("The file bfg to be saved cannot be open \n");
            exit(1);
        }
        for (int i = 0; i < maxGen; i++)
        {
            fprintf(Bfg_file, "%.3f ", file_best_fitness_gen[i]);
        }
        fclose(Bfg_file);
        // for LinkageLearning
        // Number of edges of the eVIG along the generations
        if (LL_flag > 0)
        {
            FILE* Nedges_gen_file;
            sprintf(name, "data/nedgesgen_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
            if ((Nedges_gen_file = fopen(name_p, "w")) == nullptr)
            {
                puts("The file nedgesgen to be saved cannot be open \n");
                exit(1);
            }
            for (int i = 0; i < maxGen; i++)
            {
                fprintf(Nedges_gen_file, "%.3f ", file_n_edges_eVIG_gen[i]);
            }
            fclose(Nedges_gen_file);
        }
    }

    // for LinkageLearning
    // Mean number of edges of the eVIG
    if (LL_flag > 0)
    {
        FILE* Nedges_file;
        sprintf(name, "data/nedges_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
        if ((Nedges_file = fopen(name_p, "w")) == nullptr)
        {
            puts("The file nedges to be saved cannot be open \n");
            exit(1);
        }
        for (int i = 0; i < total_runs; i++)
        {
            fprintf(Nedges_file, "%.14f ", file_n_edges_eVIG[i]);
        }
        fclose(Nedges_file);
    }

    // Best fitness
    sprintf(name, "data/bfi_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Bestfit_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file bfi to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Bestfit_file, "%.14f ", file_best_fitness[i]);
    }
    fclose(Bestfit_file);

    // Best individuals
    if (save_datagen_flag == 1)
    {
        FILE* Bestind_file;
        sprintf(name, "data/bind_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
        if ((Bestind_file = fopen(name_p, "w")) == nullptr)
        {
            puts("The file bind to be saved cannot be open \n");
            exit(1);
        }
        for (int i = 0; i < total_runs; i++)
        {
            for (int gene = 0; gene < lcrom; gene++)
                fprintf(Bestind_file, "%.10f ", File_best_ind[i][gene]);
            fprintf(Bestind_file, "\n");
        }
        fclose(Bestind_file);
    }

    // Time for each run
    sprintf(name, "data/time_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Time_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file time to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Time_file, "%.2f ", time_run[i]);
    }
    fclose(Time_file);

    // Number of generations for each run
    sprintf(name, "data/gen_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Gen_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file gen to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Gen_file, "%d ", file_gen[i]);
    }
    fclose(Gen_file);

    // Number of fitness evaluations for each run
    if (save_datagen_flag == 1)
    {
        FILE* Nfeval_file;
        sprintf(name, "data/nfeval_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
        if ((Nfeval_file = fopen(name_p, "w")) == nullptr)
        {
            puts("The file nfeval to be saved cannot be open \n");
            exit(1);
        }
        for (int i = 0; i < total_runs; i++)
        {
            fprintf(Nfeval_file, "%d ", file_nfeval[i]);
        }
        fclose(Nfeval_file);
    }

    // Best fitness at different times
    sprintf(name, "data/bfi002_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Bestfit002_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file bfi002 to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Bestfit002_file, "%.14f ", file_best_fitness_002[i]);
    }
    fclose(Bestfit002_file);
    sprintf(name, "data/bfi020_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Bestfit020_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file bfi020 to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Bestfit020_file, "%.14f ", file_best_fitness_020[i]);
    }
    fclose(Bestfit020_file);
    sprintf(name, "data/bfi040_f_N%d_ll%d_m%d_c%d.dat", N, LL_flag, mutation_type, crossover_type);
    if ((Bestfit040_file = fopen(name_p, "w")) == nullptr)
    {
        puts("The file bfi040 to be saved cannot be open \n");
        exit(1);
    }
    for (int i = 0; i < total_runs; i++)
    {
        fprintf(Bestfit040_file, "%.14f ", file_best_fitness_040[i]);
    }
    fclose(Bestfit040_file);
}
