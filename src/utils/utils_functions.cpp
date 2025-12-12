//
// Created by Daniel on 19/09/2025.
//
#include <cmath>
#include "../parameters/Parameters.h"
#include <cstdlib>
#include <random>

using namespace std;

double* aloc_vectord(const int lines)
{
    auto* vector = new double[lines]();

    return vector;
}

individuo* aloc_vectorind(const int lines)
{
    auto* vector = new individuo[lines]();

    return vector;
}

double** aloc_matrixd(const int lines, const int collums)
{
    auto** Matrix = new double*[lines]();
    for (int i = 0; i < lines; i++)
    {
        Matrix[i] = new double[collums]();
    }

    return Matrix;
}

void desaloc_matrixd(double** Matrix, const int lines)
{
    for (int i = 0; i < lines; i++)
    {
        delete[] Matrix[i];
    }
    delete[] Matrix;
}

double random_dou()
{
    return rand() / static_cast<double>(RAND_MAX);
}


int random_int(const int L_range, const int H_range)
{
    return static_cast<int>(rand() / (RAND_MAX + 1.0) * (H_range - L_range + 1) + L_range);
}

double normEuc(const double* x, const int l)
{
    double norm = 0.0;

    for (int i = 0; i < l; i++)
        norm += x[i] * x[i];

    return sqrt(norm);
}

void multMatrixVect(double* y, double** A, const int l_A, const int c_A, const double* x, const int l_x)
{
    if (c_A != l_x)
    {
        //cout << "Error - multiplication: size of matrix or vector!" << endl;
        exit(1);
    }

    for (int i = 0; i < l_A; i++)
    {
        y[i] = 0.0;
        for (int j = 0; j < c_A; j++)
        {
            y[i] = y[i] + A[i][j] * x[j];
        }
    }
}

double largEig(double** M, int l, int c)
{
    int i;
    double temp;

    double* x = aloc_vectord(c);
    double* y = aloc_vectord(c);

    for (i = 0; i < c; i++)
    {
        x[i] = 1.0;
    }

    double b = normEuc(x, c);
    do
    {
        multMatrixVect(y, M, l, c, x, c); // y=Mx
        temp = b;
        b = normEuc(y, c); // ||y||



        for (i = 0; i < c; i++)
            x[i] = y[i] / b;
    }
    while (fabs(b - temp) > 0.0001);

    delete[] y;
    delete[] x;

    return b > 0.0001 ? b : 0.0001;
}

int* aloc_vectori(const int lines)
{
    const auto vector = new int[lines];

    return vector;
}

int** aloc_matrixi(int lines, int collums)
{
    const auto Matrix = new int*[lines];
    for (int i = 0; i < lines; i++)
    {
        Matrix[i] = new int[collums];
    }

    return Matrix;
}

void desaloc_matrixi(int** Matrix, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        delete[] Matrix[i];
    }
    delete[] Matrix;
}

void rand_perm_size(const int* inp, int* out, const int size_inp, const int size_out)
{
    int* auxv = aloc_vectori(size_inp);

    for (int i = 0; i < size_inp; i++)
        auxv[i] = inp[i];

    for (int i = 0; i < size_out; i++)
    {
        int j = random_int(i, size_inp - 1);
        const int aux = auxv[i];
        auxv[i] = auxv[j];
        auxv[j] = aux;
        out[i] = auxv[i];
    }

    delete[] auxv;
}
