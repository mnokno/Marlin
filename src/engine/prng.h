//
// Created by Jakub Nowacki on 20/11/2022.
//

#ifndef MARLIN_PRNG_H
#define MARLIN_PRNG_H

#include "types.h"

namespace engine {

    class PRNG {
    public:
        PRNG(); // 1070372 default
        PRNG(ulong seed);
        ulong nextUlong();
    private:
        ulong state;
    };

} // engine

#endif //MARLIN_PRNG_H
