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
    long double nextGaussian(long double m, long double sd);
    [[nodiscard]] long double nextFloat(long double min, long double max) const;
    [[nodiscard]] long double nextFloat() const;
    [[nodiscard]] long getidum() const;

private:
    long double (*ran)(long *idum);
    long *idum;

    bool hasNextGaussian;
    float nextNextGaussian{};
};


#endif //ESN_AGANDDEWLL_RATOVIRTUAL_RANDOM_H