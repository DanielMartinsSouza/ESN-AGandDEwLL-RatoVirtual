//
// Created by Daniel on 19/09/2025.
//

#include "Simulador.h"
#include "../parameters/Parameters.h"
#include <cmath>

Simulador::Simulador(const double tamX, const double tamY)
{
    this->tamX = tamX;
    this->tamY = tamY;
    this->raio = 10;
    this->posX = 100;
    this->posY = 60;
    this->ang = 0;
    pX[0] = 100;
    pY[0] = 60;
} // construtor

//-----------------	simulador -----------------//
/*Simulador::Simulador(const double tamX, const double tamY, const int raio, const double posX, const double posY,
                     const int ang)
{
    this->tamX = tamX;
    this->tamY = tamY;
    this->raio = raio;
    this->posX = posX;
    this->posY = posY;
    this->ang = ang;
} // construtor*/

//-----------------	~simulador -----------------//
Simulador::~Simulador()
= default; // destrutor

//-----------------	execute -----------------//
/*
 * retuen true: mov executado com sucesso, false caso contrсrio
 */
bool Simulador::execute(const int acao, const int dist, double* acoes, const int step)
{
    /*
        *Aчуo
    1 -- vira 45К
    2 -- vira -45К
    3 -- vira 90К
    4 -- anda p/ frente 10 cm
    */
    switch (acao)
    {
    case 0:
        /** Bloco Rotacionar 45А **/
        ang = ((int)ang + 45) % 360;
        acoes[step] = 0;
        break;

    case 1:
        /** Bloco Rotacionar -45А **/
        ang = ((int)ang + 315) % 360;
        acoes[step] = 1;
        break;

    case 2:
        /** Bloco Rotacionar 90А **/
        ang = ((int)ang + 90) % 360;
        acoes[step] = 2;
        break;

    case 3:
        if (sensor(dist, ang) == 0)
        {
            acoes[step] = 3;
            /** Mover para frente 10cm	 M = 1	**/
            posX = posX + dist * cos(ang * PI / 180.0);
            posY = posY + dist * sin(ang * PI / 180.0);
            // vetores com tamanho 300 ou 1800
        }
        else
        {
            acoes[step] = 4;
        }
        break;
    default: ;
    } // switch
    st_pos++;
    pX[st_pos] = posX; // st = step
    pY[st_pos] = posY;
    return true;
} // execute

//-----------------	readSensor -----------------//
double* Simulador::readSensor(const int dist) const
{
    auto* sensores = new double[4];
    sensores[0] = sensor(dist, ang - 45); // direita
    sensores[1] = sensor(dist, ang); // frontal
    sensores[2] = sensor(dist, ang + 45); // esquerda
    //sensores[3] = 0;					  // cima

    return sensores;
} // readSensor

//-----------------	sensor -----------------//
int Simulador::sensor(const int dist, const int ang_param) const
{
    const double anguloRad = ang_param * PI / 180.0;

    if (const int distancia = dist + raio; posX + distancia * cos(anguloRad) >= tamX || posX + distancia *
        cos(anguloRad) <= 0 ||
        posY + distancia * sin(anguloRad) >= tamY || posY + distancia * sin(
            anguloRad) <= 0)
        return 1;
    return 0;
} // sensor
