//
// Created by kubaa on 19/11/2022.
//

#ifndef MARLIN_PRECALCULATED_DATA_H
#define MARLIN_PRECALCULATED_DATA_H

#include <bits/stdc++.h>
#include "types.h"

namespace engine {

    class PrecalculatedData {
    public:
        inline static std::list<ulong> winingLinesMasks[2][42];
        inline static ulong columMasks[7];
        inline static ulong moveMasks[42];
        static void init();
        static std::string format(ulong number);

    private:
        static ulong flipBit(ulong map, int bitToFlip);
    };

} // engine

#endif //MARLIN_PRECALCULATED_DATA_H
