//
// Created by Daniel on 19/09/2025.
//
#include <cstdlib>

#include "../../parameters/Parameters.h"
#include "../../core/Simulador.h"

class Simulador;

double calcFitness(const alelo *indiv)
{
	if (stop_flag == 0)
		nfeval++;
	if (stop_cr == 1) {
		if (nfeval >= max_nfeval)
			stop_flag = 1;
	}

	// Inicializar matrizes para armazenar ativações ao longo dos passos
	reservoir_activations_steps = new double *[nsteps];
	output_activations_steps = new double *[nsteps];
	for (int i = 0; i < nsteps; i++)
	{
		reservoir_activations_steps[i] = new double[reservoir_size];
		output_activations_steps[i] = new double[n_out];
	}
	double Fitness = 0;
	auto *simulador = new Simulador(200, 120);
	// variaveis
	constexpr int memory = 10;
	double prevRd[memory][8];

	for (auto & i : prevRd)
		for (double & j : i)
			j = -1;

	// Pesos do repositorio
	esn->setWout(indiv);
	esn->clean_z();

	// variaveis para entender o resultado
	int ganhouDireto = 0;
	int ganhouProbMem = 0;
	int perdeuBeta = 0;

	int n0 = 0;
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	auto *acoes = new double[nsteps];

	st_pos = 0;

	for (int step = 0; step < nsteps; step++)
	{
		constexpr double alpha = 0.07;
		act = false;

		const double *in10 = simulador->readSensor(10);
		const double *in30 = simulador->readSensor(30);

		auto *in = new double[8];
		for (int j = 0; j < 4; j++)
		{
			in[j] = in10[j];
			in[j + 4] = in30[j];
		}

		int sum_in = 0;
		for (int i = 0; i < input_size; i++)
			sum_in += static_cast<int>(in[i]);

		auto *out = new double[n_out];

		esn->move(in, out);

		// Define qual movimento vai ser executado (movimento correspondete a maior saida)
		int action = 0;
		int action2 = 0;

		for (int i = 1; i < n_out; i++)
		{
			if (out[i] > out[action])
			{
				action2 = action;
				action = i;
			}
			else
			{
				if (out[i] > out[action2])
					action2 = i;
			}
		}

		delete[] out;

		simulador->execute(action, 10, acoes, step);

		int acao;
		if (act)
			acao = action2;
		else
			acao = action;

		// SWITCH
		switch (acao)
		{
		case 0:
			/** Bloco Rotacionar 45° **/
			n0++;
			break;

		case 1:
			/** Bloco Rotacionar -45° **/
			n1++;
			break;

		case 2:
			/** Bloco Rotacionar 90° **/
			n2++;
			break;

		case 3:
			/** Mover para frente 10cm	 M = 1	**/
			n3++;
			// penalizado por passar em lugares ja visitados
			int equal = 0;
			for (int i = 0; i < memory; i++)
			{
				// para comecar do mais recente
				int m = (step % memory) - 1 - i;
				if (m < 0)
					m = memory + m;

				equal = 1;
				for (int s = 0; s < 8; s++)
				{
					if (prevRd[m][s] != in[s])
					{
						equal = 0;
						break;
					}
				}
				if (equal)
				{
					constexpr double gama = 0.9;
					const int m_linha = (memory - (step % memory) + m) % memory;

					const double prob = 1 - gama / (memory - m_linha);
					if (const double r = rand() / static_cast<float>(RAND_MAX); r < prob)
					{
						Fitness++;
						ganhouProbMem++;
					}
					break;
				}
			}
			// recompensado por andar pra frente
			if (!equal)
			{
				Fitness++;
				ganhouDireto++;
			}
			break;
		} // switch

		// memoria do rato - n(memory) ultimas leituras dos sensores (n passos)
		for (int k = 0; k < 8; k++)
			prevRd[step % memory][k] = in[k];

		// srand(time(NULL));

		if (const double z = rand() / static_cast<float>(RAND_MAX); z < alpha / (sum_in + 1))
		{
			constexpr double beta = 0.5;
			Fitness -= beta;
			perdeuBeta++;
		}
\
		// Salvar ativações do reservatório e da saída para o passo atual
		for (int i = 0; i < reservoir_size; i++)
		{
			reservoir_activations_steps[step][i] = reservoir_activations[i];
		}
		for (int i = 0; i < n_out; i++)
		{
			output_activations_steps[step][i] = output_activations[i];
		}

		delete[] in;
		delete[] in10;
		delete[] in30;
	}
	delete simulador;

	// para as posicoes e o numero de passos pra frente

	if (Fitness > bestFitness)
	{
		bestFitness = Fitness;
		// passando os valores do individuo anterior para os aux
		double *auxX = bestX;
		double *auxY = bestY;

		// passando os valores do individuo atual para as respectivas variaveis
		bestX = pX;
		bestY = pY;

		pX = auxX;
		pY = auxY;

		nRot45 = n0;
		nRotInv45 = n1;
		nRot90 = n2;
		nForward = n3;

		gDireto = ganhouDireto;
		gProbMem = ganhouProbMem;
		pBeta = perdeuBeta;

		// Atualizar as ativações globais para o melhor fitness
		for (int step = 0; step < nsteps; step++)
		{
			bestAcoes[step] = acoes[step];
			for (int i = 0; i < reservoir_size; i++)
			{
				best_reservoir_activations_steps[step][i] = reservoir_activations_steps[step][i];
			}
			for (int i = 0; i < n_out; i++)
			{
				best_output_activations_steps[step][i] = output_activations_steps[step][i];
			}
		}
	}

	// Desalocar matrizes de ativações
	for (int i = 0; i < nsteps; ++i)
	{
		delete[] reservoir_activations_steps[i];
		delete[] output_activations_steps[i];
	}
	delete[] reservoir_activations_steps;
	delete[] output_activations_steps;

	delete[] acoes;

	return Fitness / static_cast<double>(nsteps);
}