//
// Created by Daniel on 19/09/2025.
//

#ifndef ESN_AGANDDEWLL_RATOVIRTUAL_RANDOM_H
#define ESN_AGANDDEWLL_RATOVIRTUAL_RANDOM_H

#define RAN0 0
#define RAN1 1
#define RAN2 2

class Random {
public:
    Random(int function, long seed);

    ~Random();

    [[nodiscard]] int nextInt(int max) const;

    [[nodiscard]] bool nextBool() const;

    double nextGaussian(double m, double sd);

    [[nodiscard]] double nextFloat(double min, double max) const;

    [[nodiscard]] double nextFloat() const;

    [[nodiscard]] long getidum() const;

private:
    long double (*ran)(long *idum);

    long *idum;

    bool hasNextGaussian;
    double nextNextGaussian{};
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_RANDOM_H
