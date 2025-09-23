//
// Created by Daniel on 19/09/2025.
//

#include "Random.h"
#include <cmath>
#include "nr_ran.h"

Random::Random(const int function, const long seed)
{
    idum = new long;
    *idum = seed;

    hasNextGaussian = false;

    switch (function)
    {
        case RAN0:
            ran = ran0;
            break;
        case RAN1:
            *idum *= (seed > 0) ? -1 : 1; // guarantees idum < 0.
            ran = ran1;
            break;
        case RAN2:
            ran = ran2;
        default: ;
    }
}

Random::~Random()
{
    delete idum;
}

int Random::nextInt(const int max) const
{
    return static_cast<int>(floor(max * nextFloat()));
}

bool Random::nextBool() const
{
    return nextFloat() < 0.5;
}

long double Random::nextGaussian(const long double m, const long double sd)
{
    long double next;

    if (hasNextGaussian) // use value from previous call
    {
        next = nextNextGaussian;
        hasNextGaussian = false;
    }
    else
    {
        long double x1, x2, w;

        do
        {
            x1 = 2.0 * nextFloat() - 1.0;
            x2 = 2.0 * nextFloat() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0 || w == 0);

        w = sqrt((-2.0 * log(w)) / w);
        next = x1 * w;
        nextNextGaussian = x2 * w;
        hasNextGaussian = true;
    }

    return (m + next * sd);
}

long double Random::nextFloat(const long double min, const long double max) const
{
    return nextFloat() * (max - min) + min;
}

long double Random::nextFloat() const {
    return (*ran)(idum);
}

long Random::getidum() const {
    return *idum;
}
