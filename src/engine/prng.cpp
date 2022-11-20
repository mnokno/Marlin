//
// Created by kubaa on 20/11/2022.
//

#include "prng.h"

namespace engine {

    PRNG::PRNG() {
        this->state = (ulong)1070372;
    }

    PRNG::PRNG(ulong seed) {
        this->state = seed;
    }

    // implementation of xorshift64star Pseudo-Random Number Generator
    ulong PRNG::nextUlong() {
        state ^= state >> (ulong)12;
        state ^= state << (ulong)25;
        state ^= state >> (ulong)27;
        return state * (ulong)2685821657736338717;
    }

} // engine