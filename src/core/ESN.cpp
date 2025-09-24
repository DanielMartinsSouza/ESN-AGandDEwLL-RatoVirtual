//
// Created by Daniel on 19/09/2025.
//

#include "ESN.h"

#include <cmath>

#include "../parameters/Parameters.h"
#include <cstdlib>

ESN::ESN(const int reservoir_size, const double sparsity, const double spectral_radius_d, const int n_out, const int input_size, const int n_examples, const int n_stab, const int seed = 10)
{
    this->reservoir_size = reservoir_size;
    this->sparsity = sparsity;
    this->input_size = input_size;
    this->n_out = n_out;
    this->n_examples = n_examples;
    this->n_stab = n_stab;
    this->spectral_radius_d = spectral_radius_d;

    W_in = aloc_matrixd(reservoir_size, input_size);
    W = aloc_matrixd(reservoir_size, reservoir_size);
    W_out = aloc_matrixd(n_out, reservoir_size);

    z_old = aloc_vectord(reservoir_size);

    res_neurons = new neuron[reservoir_size];
    out_neurons = new neuron[n_out];

    reservoir_activations = new double[reservoir_size];
    output_activations = new double[n_out];

    best_reservoir_activations_steps = new double *[nsteps];
    best_output_activations_steps = new double *[nsteps];
    for (int i = 0; i < nsteps; i++)
    {
        best_reservoir_activations_steps[i] = new double[reservoir_size];
        best_output_activations_steps[i] = new double[n_out];
    }

    srand(seed);
    weights_init();
}

ESN::~ESN()
{

    desaloc_matrixd(W_in, reservoir_size);
    desaloc_matrixd(W, reservoir_size);
    desaloc_matrixd(W_out, n_out);

    delete[] res_neurons;

    delete[] out_neurons;
    delete[] z_old;

    for (int i = 0; i < nsteps; ++i)
    {
        delete[] best_reservoir_activations_steps[i];
        delete[] best_output_activations_steps[i];
    }
    delete[] best_reservoir_activations_steps;
    delete[] best_output_activations_steps;
    delete[] reservoir_activations;
    delete[] output_activations;
}

void ESN::weights_init() const {
    int i, j;
    constexpr double min_W = -0.6;
    constexpr double max_W = 0.6;

    for (i = 0; i < reservoir_size; i++)
    {
        res_neurons[i].bias = (max_W - min_W) * random_dou() + min_W; // random number between min_W and max_W
        for (j = 0; j < input_size; j++)
            W_in[i][j] = (max_W - min_W) * random_dou() + min_W; // random number between min_W and max_W
    }

    for (i = 0; i < reservoir_size; i++)
    {
        for (j = 0; j < reservoir_size; j++)
        {
            if (i == j || random_dou() > sparsity)
                W[i][j] = 0;
            else
            {
                W[i][j] = (max_W - min_W) * random_dou() + min_W;
            }
        }
    }

    const double spectral_radius = largEig(W, reservoir_size, reservoir_size);

    for (i = 0; i < reservoir_size; i++)
        for (j = 0; j < reservoir_size; j++)
            W[i][j] = spectral_radius_d * W[i][j] / spectral_radius;

}

void ESN::setWout(const double *weight) const {
    int k = 0;
    for (int i = 0; i < n_out; i++){
        out_neurons[i].bias = weight[k];
        k++;
        for (int j = 0; j < reservoir_size + 1; j++){
            W_out[i][j] = weight[k];
            k++;
        }
    }
}

void ESN::output(const double *u, double *z, double *y) const {
    int i, j;
    double sum_u;

    // Activation of the neurons in the hidden layer 1 (reservoir)
    for (i = 0; i < reservoir_size; i++)
    {
        sum_u = res_neurons[i].bias;
        for (j = 0; j < input_size; j++)
            sum_u = sum_u + u[j] * W_in[i][j];
        for (j = 0; j < reservoir_size; j++)
            sum_u = sum_u + z_old[j] * W[i][j];
        z[i] = tanh(1.0 * sum_u); // Tangent hiperbolic with half-slope a=2
        reservoir_activations[i] = z[i];
    }

    for (i = 0; i < reservoir_size; i++)
    {
        z_old[i] = z[i];
    }

    // Activation of the output units
    for (i = 0; i < n_out; i++)
    {
        sum_u = out_neurons[i].bias;

        for (j = 0; j < reservoir_size; j++)
            sum_u = sum_u + z[j] * W_out[i][j];
        y[i] = sum_u;
        // y[i]=tanh(1.0*sum_u);				// Tangent hiperbolic with half-slope 2 and input sum_u; obs.: in the original tanh, a=2
        output_activations[i] = y[i];
    }
}

void ESN::reservoir_activation(const double *u, const double *z_old, double *z) const {
    int j;

    // Activation of the neurons in hidden layer 1 (reservoir)
    for (int i = 0; i < reservoir_size; i++)
    { // neuron
        double sum_u = res_neurons[i].bias;
        for (j = 0; j < input_size; j++)
            sum_u = sum_u + u[j] * W_in[i][j];
        for (j = 0; j < reservoir_size; j++)
            sum_u = sum_u + z_old[j] * W[i][j];
        z[i] = tanh(1.0 * sum_u); // Tangent hiperbolic with half-slope a=2
    }
    // op ao pra nao ter que multiplicar os zeros
    // for (j=0 ; j<hid1_neurons[i].n_rec ; j++)
    // sum_u = sum_u + z_old[ hid1_neurons[i].rec[j] ]*W[i][ hid1_neurons[i].rec[j]  ];
}

void ESN::move(const double *in, double *out) const {
    double *z = aloc_vectord(reservoir_size);

    output(in, z, out);

    delete[] z;
}

void ESN::clean_z() const {
    for (int i = 0; i < reservoir_size; i++)
    {
        z_old[i] = 0;
    }
}

void ESN::preTrain(const double *data_s, double *z_old, double *z) const {

    int i;

    // Stabilization period
    for (i = 0; i < reservoir_size; i++)
        z_old[i] = 0.0;

    // Running the ESN for n_stab iterations
    for (int t = 0; t < n_stab; t++)
    {
        reservoir_activation(data_s, z_old, z);
        for (i = 0; i < reservoir_size; i++)
            z_old[i] = z[i];
    }
}
