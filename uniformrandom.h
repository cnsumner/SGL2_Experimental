#pragma once
#ifndef _UNIFORM_RANDOM_DEFINED_
#define _UNIFORM_RANDOM_DEFINED_

#include <random>

using std::uniform_int_distribution;
using std::random_device;
using std::mt19937;

class UniformRandomGenerator {
    //  A uniform distribution object
    uniform_int_distribution<int> dist;
    //  A Mersenne Twister random number generator with a seed 
    //  obtained from a random_device object
    mt19937 mt;
public:
    //  Create a pseudorandom number generator that produces values in 
    //  the range low...high
    UniformRandomGenerator(int low, int high) : dist(low, high),
        mt(random_device()()) {
    }

    //  Return a pseudorandom number in the range MIN...MAX
    int operator()() {
        return dist(mt);
    }
};

#endif


