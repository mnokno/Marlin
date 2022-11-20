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
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        return state * 2685821657736338717;
    }

} // engine