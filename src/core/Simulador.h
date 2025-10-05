//
// Created by Daniel on 19/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_SIMULADOR_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_SIMULADOR_H
#pragma once
#include "../parameters/Parameters.h"
#include <cmath>

//#define PI 3.14159265
#define PI 3.1415926535897932384626433832795029

class Simulador {
    [[nodiscard]] int sensor(int dist, int ang_param) const;
    double tamX; // tamanho do tabladoo eixo x
    double tamY; // tamanho do tabladoo eixo y
    double posX; // posição do centro do robô no eixo x
    double posY; // posição do centro do robô no eixo y
    int ang;	 // angulo da frente do robô em relação ao eixo x
    int raio;	 // raio do robô

public:
    Simulador(double tamX, double tamY);
    Simulador(double tamX, double tamY, int raio, double posX, double posY, int ang);
    ~Simulador();
    bool execute(int acao, int dist, double *acoes, int step);
    [[nodiscard]] double *readSensor(int dist) const;
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_SIMULADOR_H