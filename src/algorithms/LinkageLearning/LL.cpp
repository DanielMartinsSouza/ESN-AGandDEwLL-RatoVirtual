//
// Created by Daniel on 24/09/2025.
//
#include <cmath>

#include "../../core/estVIG2.h"
#include "../../parameters/Parameters.h"

double LL(alelo *cromossomo1, const alelo *cromossomo2, estVIG2 *eVIG_instance, double fitness) {
    int i;
    double fxnew;
    individuo xg, xh, xgh;

    // Selecting index g
    int g = random_int(0, lcrom - 1);
    double dxg = fabs(cromossomo1[g] - cromossomo2[g]);
    for (i = 0; dxg <= EPS2 && i < lcrom; i++) {
        g = (g + 1) % lcrom;
        dxg = fabs(cromossomo1[g] - cromossomo2[g]);
    }
    if (i == lcrom)
        return fitness;

    // Selecting index h
    int h = g;
    while (h == g)
        h = random_int(0, lcrom - 1);
    double dxh = fabs(cromossomo1[h] - cromossomo2[h]);
    for (i = 0; dxh <= EPS2 && i < lcrom; i++) {
        h = (h + 1) % lcrom;
        if (h == g) {
            h = (h + 1) % lcrom;
            i++;
        }
        dxh = fabs(cromossomo1[h] - cromossomo2[h]);
    }
    if (i >= lcrom)
        return fitness;

    xg.cromossomo = aloc_vectord(lcrom); // individal xg
    xh.cromossomo = aloc_vectord(lcrom); // individual xh
    xgh.cromossomo = aloc_vectord(lcrom); // individual xgh
    for (int gene = 0; gene < lcrom; gene++) {
        xg.cromossomo[gene] = cromossomo1[gene];
        xh.cromossomo[gene] = cromossomo1[gene];
        xgh.cromossomo[gene] = cromossomo1[gene];
    }

    // Individual xg: individual x with mutation in x[g]
    xg.cromossomo[g] = cromossomo2[g];
    //xg.fitness = compFitness(xg.chromosome);
    xg.fitness = calcFitness(xg.cromossomo);
    if (stop_flag == 1) {
        delete[] xg.cromossomo;
        delete[] xh.cromossomo;
        delete[] xgh.cromossomo;
        return fitness;
    }
    // Variable importance for variable g
    double df = fabs(xg.fitness - fitness);
    if (df > EPS4)
        eVIG_instance->wVert(g, df / dxg); // updating weight of vertex g (importance of variable g)

    // Individual xh: individual x with mutation in x[h]
    xh.cromossomo[h] = cromossomo2[h];
    xh.fitness = calcFitness(xh.cromossomo);;
    if (stop_flag == 1) {
        delete[] xg.cromossomo;
        delete[] xh.cromossomo;
        delete[] xgh.cromossomo;
        return fitness;
    }
    // Variable importance for variable h
    df = fabs(xh.fitness - fitness);
    if (df > EPS4)
        eVIG_instance->wVert(g, df / dxh); // updating weight of vertex h (importance of variable h)

    // Individual xhg: individual x with mutation in x[h] and x[g]
    xgh.cromossomo[g] = cromossomo2[g];
    xgh.cromossomo[h] = cromossomo2[h];
    xgh.fitness = calcFitness(xgh.cromossomo);
    if (stop_flag == 1) {
        delete[] xg.cromossomo;
        delete[] xh.cromossomo;
        delete[] xgh.cromossomo;
        return fitness;
    }
    // Variable interaction
    df = fabs(xgh.fitness - xh.fitness - xg.fitness + fitness);
    if (df > EPS4) {
        // df changed because variables g and h interact
        // Variable interaction for variables g and h
        double dxgh = sqrt(dxg * dxg + dxh * dxh); // Euclidean distance (size of the difference vector)

        eVIG_instance->addEdge(g, h, df / dxgh);
        // adding edge (g,h) to eVIG if the edge does not exist; otherwise, update the weight of the edge
        // Variable importance for variable H
        df = fabs(xgh.fitness - xg.fitness);
        if (df > EPS4)
            eVIG_instance->wVert(h, df / dxh); // updating weight of vertex h (importance of variable h)
        // Variable importance for variable g
        df = fabs(xgh.fitness - xh.fitness);
        if (df > EPS4)
            eVIG_instance->wVert(g, df / dxg); // updating weight of vertex g (importance of variable g)
    }

    // Updating individual x
    if (xgh.fitness > xh.fitness && xgh.fitness > xg.fitness && xgh.fitness > fitness) {
        // best individual: xgh
        cromossomo1[g] = xgh.cromossomo[g];
        cromossomo1[h] = xgh.cromossomo[h];
        fxnew = xgh.fitness;
    } else if (xg.fitness > xh.fitness && xg.fitness > fitness) {
        // best individual: xg
        cromossomo1[g] = xg.cromossomo[g];
        fxnew = xg.fitness;
    } else if (xh.fitness > fitness) {
        // best individual: xh
        cromossomo1[h] = xh.cromossomo[h];
        fxnew = xh.fitness;
    } else {
        // best individual: x
        fxnew = fitness;
    }

    delete[] xg.cromossomo;
    delete[] xh.cromossomo;
    delete[] xgh.cromossomo;

    return fxnew;
}
