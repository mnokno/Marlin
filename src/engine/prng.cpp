//
// Created by Jakub Nowacki on 20/11/2022.
//

#include "prng.h"

namespace engine {

    /**
     * Creates a new instance of PRNG with a predefined seed.
     * implementation of xorshift64star Pseudo-Random Number Generator
    */
    PRNG::PRNG() {
        this->state = (ulong)1070372;
    }

    /**
     * Creates a new instance of PRNG with a given seed.
     * implementation of xorshift64star Pseudo-Random Number Generator
     *
     * @param seed The seed to use.
    */
    PRNG::PRNG(ulong seed) {
        this->state = seed;
    }

    /**
     * Returns the next random number,
     *
     * @return The next random number.
     */
    ulong PRNG::nextUlong() {
        state ^= state >> (ulong)12;
        state ^= state << (ulong)25;
        state ^= state >> (ulong)27;
        return state * (ulong)2685821657736338717;
    }

} // engine