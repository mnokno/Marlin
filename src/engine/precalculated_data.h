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
        inline static std::list<ulong> winingLinesMasks[42];
        inline static ulong columMasks[7];
        inline static ulong moveMasks[42];
        inline static ulong adjacencySquareMasks[42];
        inline static ulong boarderMasks[4];
        static void init();
        static std::string formatUlong(ulong number);
        static std::string formatBoard(ulong number);
    private:
        static bool isOnBoard(int pos);
        static bool isOnEdge(int pos);
        static bool hasWrappedOver(int oldPos, int newPos);
    };

} // engine

#endif //MARLIN_PRECALCULATED_DATA_H
