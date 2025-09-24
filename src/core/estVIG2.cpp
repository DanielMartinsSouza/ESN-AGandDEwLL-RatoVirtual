//
// Created by Daniel on 23/09/2025.
//

#include "estVIG2.h"

#include <cmath>
#include <iostream>
#include "../parameters/Parameters.h"
#include "Graph.h"

using namespace std;

estVIG2::estVIG2(const int N, const double CR, const int LL_f, const int cr_type_f)
{

	this->CR = CR;
	// Parameters for eVIG
	this->N = N;
	this->LL_f = LL_f;
	this->cr_type = cr_type_f;
	n_edges_eVIG = 0;
	if (LL_f > 0)
	{

		sum_weightEdge = 0.0;
		max_k = 10;	  // Maximum number of edges stored for each vertex for VIntX
		max_k_cr = 2; // Maximum number of edges used for each vertex for VIntX in Grec

		// Create adjacenty matrix for Variable Interaction Graph (VIT
		Madj = aloc_matrixd(N, N);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				Madj[i][j] = 0.0;

		max_weightEdge = 0.0;
		// Weights of the vertices(variable importance)
		M_max_weigths = aloc_matrixd(N, max_k);	  // allocation of the matrix
		M_max_weigths_v = aloc_matrixi(N, max_k); // allocation of the matrix
		weightVert = new double[N];
		size_M_max_weigths = new int[N];
		if (LL_f == 1 && cr_type == 2)
		{
			for (int i = 0; i < N; i++)
			{
				weightVert[i] = 0.0; // initially, the variable importance is zero
				size_M_max_weigths[i] = 0;
				for (int j = 0; j < max_k; j++)
				{
					M_max_weigths[i][j] = 0.0;
					M_max_weigths_v[i][j] = -1;
				}
			}
		}

		// statistics
		n_cross = 0;
		n_comp_avg = 0;
		max_size_avg = 0;
	}
}

/******************************************************************************\
*								 Destructor									   *
\******************************************************************************/
estVIG2::~estVIG2()
{

	if (LL_f > 0)
	{
		delete[] weightVert;
		delete[] size_M_max_weigths;
		desaloc_matrixd(Madj, N);
		desaloc_matrixi(M_max_weigths_v, N);
		desaloc_matrixd(M_max_weigths, N);
	}
}

/******************************************************************************\
*								Print interaction information				   *
\******************************************************************************/
void estVIG2::print() const {

	cout << "eVIG: variables " << endl;
	for (int i = 0; i < N; i++)
	{
		cout << " x_" << i << ": ";
		for (int j = 0; j < N; j++)
		{
			if (Madj[i][j] > 0.0)
			{
				cout << "  x_" << j;
				cout << "(" << Madj[i][j] << ")";
			}
			cout << endl;
		}
	}
}

void estVIG2::save(double **Madj_s, const double *weightVert_s, const int ll_type, const int m_type, const int c_type) const {
	FILE *eVIGm_edge_file;
	char name[1000];
	double sum_M;

	const char *name_p = name;

	// Saving the adjacency matrix (edges) - the sum should be equal to 1
	sprintf(name, "data/eVIGm_edge_f_N%d_ll%d_m%d_c%d.csv", N, ll_type, m_type, c_type);
	if ((eVIGm_edge_file = fopen(name_p, "w")) == NULL)
	{
		puts("The file eVIGm_edge to be saved cannot be open \n");
		exit(1);
	}

	if (LL_f == 1)
	{
		sum_M = 0.0;
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				sum_M += Madj_s[i][j];
	}
	else
	{
		sum_M = 1.0;
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j < N - 1)
				fprintf(eVIGm_edge_file, "%1.8f, ", Madj_s[i][j] / sum_M);
			else
				fprintf(eVIGm_edge_file, "%1.8f", Madj_s[i][j] / sum_M);
		}
		if (i < N - 1)
			fprintf(eVIGm_edge_file, "\n");
	}
	fclose(eVIGm_edge_file);

	if (LL_f == 1)
	{
		FILE *eVIGm_vert_file;
		// Saving the weights of the vertices
		sprintf(name, "data/eVIGm_vert_f_N%d_ll%d_m%d_c%d.csv", N, ll_type, m_type, c_type);
		if ((eVIGm_vert_file = fopen(name_p, "w")) == NULL)
		{
			puts("The file eVIGm_vert to be saved cannot be open \n");
			exit(1);
		}
		double sum_w = 0.0;
		for (int i = 0; i < N; i++)
			sum_w += weightVert_s[i];
		for (int i = 0; i < N; i++)
		{
			if (i < N - 1)
				fprintf(eVIGm_vert_file, "%1.8f, ", weightVert_s[i] / sum_w);
			else
				fprintf(eVIGm_vert_file, "%1.8f", weightVert_s[i] / sum_w);
		}
		fclose(eVIGm_vert_file);
	}
}

/******************************************************************************\
* 		Update weight of vertex a with weight w (variable importance)	  	   *
\******************************************************************************/
void estVIG2::wVert(const int a, const double w)
{

	if (w > weightVert[a])
	{
		weightVert[a] = w; //  weight of the vertex (variable importance) is the maximum (for all calls of wVert(int a, double w))
		if (w > max_weightVert)
			max_weightVert = w; // update maximum vertex weight
	}
}

/******************************************************************************\
* 		Add edge (a,b) to the eVIG with weight w (2-variable interaction)	   *
\******************************************************************************/
void estVIG2::addEdge(int a, int b, double w)
{
	if (w > Madj[a][b])
	{
		if (Madj[a][b] <= EPS2)
			n_edges_eVIG += 2;
		const double w_old = Madj[a][b];
		Madj[a][b] = w;
		Madj[b][a] = w;

		sum_weightEdge = sum_weightEdge - 2 * w_old + 2 * w; // update sum of the weights
		if (w > max_weightEdge)
			max_weightEdge = w; // update maximum edge weight

		if (cr_type == 2)
		{
			int j;
			int max_k_1;
			// Update M_max_weigths

			// for vertex a
			if (size_M_max_weigths[a] == 0)
			{
				// no edges for vertex a stored yet
				M_max_weigths[a][0] = w;
				M_max_weigths_v[a][0] = b;
				size_M_max_weigths[a]++;
			}
			else
			{
				// check and remove edge a to b if it is in M_max_weigths,
				max_k_1 = size_M_max_weigths[a] - 1;
				j = max_k_1;
				while (j >= 0 && b != M_max_weigths_v[a][j])
					j--;
				if (j >= 0)
				{
					for (int i = j; i < max_k_1; i++)
					{
						M_max_weigths[a][i] = M_max_weigths[a][i + 1];
						M_max_weigths_v[a][i] = M_max_weigths_v[a][i + 1];
					}
					M_max_weigths[a][max_k_1] = 0.0;
					M_max_weigths_v[a][max_k_1] = -1;
					size_M_max_weigths[a]--;
				}
				// add edge a to b if w is among the maximum weights
				j = max_k - 1;
				while (j >= 0 && w > M_max_weigths[a][j])
				{
					if (j > 0)
					{
						M_max_weigths[a][j] = M_max_weigths[a][j - 1];
						M_max_weigths_v[a][j] = M_max_weigths_v[a][j - 1];
					}
					j--;
				}
				if (j < max_k - 1)
				{
					M_max_weigths[a][j + 1] = w;
					M_max_weigths_v[a][j + 1] = b;
					if (size_M_max_weigths[a] < max_k)
						size_M_max_weigths[a]++;
				}
			}

			// for vertex b
			if (size_M_max_weigths[b] == 0)
			{
				// no edges for vertex b stored yet
				M_max_weigths[b][0] = w;
				M_max_weigths_v[b][0] = a;
				size_M_max_weigths[b]++;
			}
			else
			{
				// check and remove edge b to a if it is in M_max_weigths,
				max_k_1 = size_M_max_weigths[b] - 1;
				j = max_k_1;
				while (j >= 0 && b != M_max_weigths_v[b][j])
					j--;
				if (j >= 0)
				{
					for (int i = j; i < max_k_1; i++)
					{
						M_max_weigths[b][i] = M_max_weigths[b][i + 1];
						M_max_weigths_v[b][i] = M_max_weigths_v[b][i + 1];
					}
					M_max_weigths[b][max_k_1] = 0.0;
					M_max_weigths_v[b][max_k_1] = -1;
					size_M_max_weigths[b]--;
				}
				// add edge b to a if w is among the maximum weights
				j = max_k - 1;
				while (j >= 0 && w > M_max_weigths[b][j])
				{
					if (j > 0)
					{
						M_max_weigths[b][j] = M_max_weigths[b][j - 1];
						M_max_weigths_v[b][j] = M_max_weigths_v[b][j - 1];
					}
					j--;
				}
				if (j < max_k - 1)
				{
					M_max_weigths[b][j + 1] = w;
					M_max_weigths_v[b][j + 1] = a;
					if (size_M_max_weigths[b] < max_k)
						size_M_max_weigths[b]++;
				}
			}
		}
	}
}

/******************************************************************************\
* 		Crossover statistics: average number of components (greater than 1)    *
\******************************************************************************/
double estVIG2::avgNComp() const
{

	if (n_cross > 0)
		return (static_cast<double>(n_comp_avg) / n_cross);
	return (0.0);
}

/******************************************************************************\
* 		Crossover statistics: average maximum component size	  	   		   *
\******************************************************************************/
double estVIG2::avgMCompSize() const
{

	if (n_cross > 0)
		return (static_cast<double>(max_size_avg) / n_cross);
	return (0.0);
}

/*******************************************************************************************\
* 		Real Variable Interaction Crossover (VIntX) for DE						   			*
\*******************************************************************************************/
void estVIG2::VIntCrossover_DE(const double *parent1, const double *parent2, double *offspring)
{
	int i, n_vert = 0;

	const auto comp_id = new int[N];	   // indicates the recombining component associated with each variable
	const auto map_var2vert = new int[N]; // vector used to map variables to vertices
	const auto map_vert2var = new int[N]; // vector used to map vertices to variables
	// statistics
	n_cross++; // number of times that crossover is applied

	// Assigning common variables
	for (i = 0; i < N; i++)
	{
		// if ( parent1[i]!=parent2[i] ){
		if (fabs(parent1[i] - parent2[i]) > EPS4)
		{
			map_var2vert[i] = n_vert;
			map_vert2var[n_vert] = i;
			n_vert++;
		}
		else
		{
			map_var2vert[i] = -1;
			if (random_dou() < CR)
			{
				offspring[i] = parent1[i];
			}
			else
			{
				offspring[i] = parent2[i];
			}
		}
	}

	if (n_vert > 0)
	{
		list<int>::iterator ii;
		int max_k_aux;
		int max_size = 0;
		int j;

		// Step 1: Creating the recombination graph
		//  The recombination graph is created with a maximum of max_k*N edges (see observation in the definition of max_k) --> this is O(N max_k) -> because max_k is O(1), then it is O(N)
		//  remember that the max_k -th maximum weight of the i-th vertex is M_max_weigths[i][max_k-1]

		auto *G_rec = new Graph(n_vert); // recombination graph
		for (int var_index = 0; var_index < n_vert; var_index++)
		{
			i = map_vert2var[var_index];
			if (max_k_cr > size_M_max_weigths[i])
			{
				max_k_aux = size_M_max_weigths[i];
			}
			else
			{
				max_k_aux = max_k_cr;
			}
			for (int k = 0; k < max_k_aux; k++)
			{
				if (max_k_cr >= size_M_max_weigths[i])
				{
					j = M_max_weigths_v[i][k];
				}
				else
				{
					j = M_max_weigths_v[i][random_int(0, size_M_max_weigths[i] - 1)];
				}
				if (const int var_index_next = map_var2vert[j]; var_index_next > -1 && G_rec->isThereEdge(var_index, var_index_next) == 0)
				{
					G_rec->addUndirectedEdge(var_index, var_index_next);
				}
			}
		}

		// Step 2: Finding the connected components of G_rec
		// G_rec->printGraph();
		G_rec->connectedComponents(comp_id);

		int n_comp = 0; // number of components
		for (i = 0; i < n_vert; i++)
			if (comp_id[i] > n_comp)
				n_comp = comp_id[i];
		n_comp++; // remember that the first component has label 0

		auto *comp = new recomb_comp[n_comp]; // recombining components
		for (i = 0; i < n_comp; i++)
		{
			comp[i].size = 0;
		}

		for (i = 0; i < n_vert; i++)
		{
			comp[comp_id[i]].size += 1;
		}
		for (i = 0; i < n_vert; i++)
		{
			j = map_vert2var[i];
			const int c = comp_id[i];
			comp[c].var_id.push_back(j);
		}

		// Step 3: Generating offspring
		for (i = 0; i < n_comp; i++)
		{
			// statistics
			if (comp[i].size > max_size)
				max_size = comp[i].size;
			if (comp[i].size > 1)
				n_comp_avg++;
			if (random_dou() < CR)
			{
				// Variables in the i-th component for the offspring come from parent 1
				ii = comp[i].var_id.begin();
				while (ii != comp[i].var_id.end())
				{
					offspring[*ii] = parent1[*ii];
					++ii;
				}
			}
			else
			{
				/// Variables in the i-th component for the offspring come from parent 2
				ii = comp[i].var_id.begin();
				while (ii != comp[i].var_id.end())
				{
					offspring[*ii] = parent2[*ii];
					++ii;
				}
			}
		}

		// statistics
		max_size_avg += max_size;

		delete[] comp;
		delete G_rec;
	}

	delete[] comp_id;
	delete[] map_var2vert;
	delete[] map_vert2var;
}

/*******************************************************************************************\
* 		Group-wise Binomial Crossover (GbinX) 						   						*
* 		Reference:																			*
*			Cai Y, Wang J. Differential evolution with hybrid linkage crossover. 			*
*				Information Sciences. 2015 Nov 1;320:244-87.								*
\*******************************************************************************************/
void estVIG2::GbinX(const double *parent1, const double *parent2, double *offspring) const {
	const double epsilon_GbinX = sum_weightEdge / (N * (N - 1)) + EPS2; // epsilon_GbinX: average weight

	// Adaptive Grouping (AG)
	const auto Z_AG = new int[N];
	const auto PI_AG = new int[N];
	const auto comp_id = new int[N];

	for (int i = 0; i < N; i++)
		Z_AG[i] = 0;
	rand_perm_size(vsort_aux, PI_AG, N, N);
	int n_comp = 0; // number of components
	for (int i = 0; i < N; i++)
	{
		if (int a = PI_AG[i]; Z_AG[a] == 0)
		{
			Z_AG[a] = 1;
			comp_id[a] = n_comp;
			for (int b = 0; b < N; b++)
			{
				if (Z_AG[b] == 0 && Madj[a][b] >= epsilon_GbinX)
				{
					comp_id[b] = n_comp;
					Z_AG[b] = 1;
				}
			}

			n_comp++;
		}
	}

	// Group-wise Binomial Crossover (GbinX)
	auto comp_set = new int[n_comp];
	for (int i = 0; i < n_comp; i++)
	{
		if (random_dou() < CR)
			comp_set[i] = 1; // variables in the i-th component for the offspring come from parent 1
		else
			comp_set[i] = 2; // variables in the i-th component for the offspring come from parent 2
	}
	for (int i = 0; i < N; i++)
	{
		if (comp_set[comp_id[i]] == 1)
			offspring[i] = parent1[i];
		else
			offspring[i] = parent2[i];
	}

	delete[] comp_set;
	delete[] Z_AG;
	delete[] PI_AG;
	delete[] comp_id;
}
