//
// Created by Daniel on 19/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_ESN_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_ESN_H


class ESN
{
    double** W_in;
    double** W;
    double** W_out;
    double* z_old;
    int reservoir_size; // number of neurons in the reservoir
    double sparsity; // percent of nonzero elements in W
    double spectral_radius_d; // maximal absolute eigenvalue of W (it scales W)
    int n_out; // number of neurons in the output layer
    int input_size; // size of the input
    int n_examples; //
    int n_stab; // number of interations for the stabilizing period

    typedef struct
    {
        double bias; // bias
    } neuron;

    neuron* res_neurons; // neurons in reservoir
    neuron* out_neurons; // neurons in output layer

    void weights_init() const;
    void reservoir_activation(const double* u, const double* z_old, double* z) const;
    void preTrain(const double* data_s, double* z_old, double* z) const;
    void output(const double* u, double* z, double* y) const;

public:
    ESN(int reservoir_size, double sparsity, double spectral_radius_d, int n_out, int input_size, int n_examples,
        int n_stab, int seed);
    ~ESN();
    void setWout(const double* weight) const;
    void move(const double* in, double* out) const;
    void clean_z() const;
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_ESN_H
