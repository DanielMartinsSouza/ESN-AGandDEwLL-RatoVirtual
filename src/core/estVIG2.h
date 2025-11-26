//
// Created by Daniel on 23/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_ESTVIG2_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_ESTVIG2_H
#include <list>

using namespace std;

class estVIG2 {
    int N; // Number of decision variables
    int cr_type; // type of crossover
    int max_k; // Maximum number of edges stored for each vertex for VIntX
    int max_k_cr; // Maximum number of edges used for each vertex for VIntX in Grec
    int LL_f; // Linkage learning (LL_f=1)
    int *size_M_max_weigths; // Number of edges stored for each vertex for VIntX
    int **M_max_weigths_v; // Vertex Matrix with the (sorted) max_k maximum weights for each vertex
    double CR; // crossover probability
    double **M_max_weigths; // Weight Matrix with the (sorted) max_k maximum weights for each vertex
    typedef struct {
        int size; // number of vertices in the recombining component
        list<int> var_id; // List that indicates variables in each component
    } recomb_comp; // recombining components for VIntX
    //void LMC(double *x_bound_f); // (offline) Ll: Linkage Matrix Construction (LMC)
public:
    int n_edges_eVIG; // number of edges in the eVIG
    long int n_cross, n_comp_avg, max_size_avg;
    // statistics to compute average number of components (greater than 1) and maximum component size
    double max_weightVert{}; // maximum (found) for vertex weight
    double max_weightEdge; // maximum (found) for edge weight
    double sum_weightEdge; // sum for all edge weights
    double *weightVert; // Vector with the weights of the vertices (variable importance)
    double **Madj; // Adjacency Matrix (2-variable interaction)
    estVIG2(int N, double CR, int LL_f, int cr_type_f);

    ~estVIG2();

    void print() const; // Print eVIG
    void save(double **Madj_s, const double *weightVert_s, int ll_type, int m_type, int c_type) const; // Save eVIG
    void wVert(int a, double w); // Update weight of vertex a twith weight w (variable importance)
    void VIntCrossover_DE(const double *parent1, const double *parent2, double *offspring);

    // Real Variable Interaction Crossover (VIntX) for DE
    void GbinX(const double *parent1, const double *parent2, double *offspring) const;

    // Group-wise Binomial Crossover (GbinX)
    void addEdge(int a, int b, double w); // Add edge (a,b) to the eVIG with weight w (2-variable interaction)
    [[nodiscard]] double avgNComp() const; // Crossover statistics: average number of components (greater than 1)
    [[nodiscard]] double avgMCompSize() const; // Crossover statistics: average maximum component size
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_ESTVIG2_H
