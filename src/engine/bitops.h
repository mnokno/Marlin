//
// Created by kubaa on 20/11/2022.
//

#ifndef MARLIN_BITOPS_H
#define MARLIN_BITOPS_H

#include "types.h"

namespace engine{
    class BitOps {
    private:
        inline static int index64[64] = {
                0, 47,  1, 56, 48, 27,  2, 60,
                57, 49, 41, 37, 28, 16,  3, 61,
                54, 58, 35, 52, 50, 42, 21, 44,
                38, 32, 29, 23, 17, 11,  4, 62,
                46, 55, 26, 59, 40, 36, 15, 53,
                34, 51, 20, 43, 31, 22, 10, 45,
                25, 39, 14, 33, 19, 30,  9, 24,
                13, 18,  8, 12,  7,  6,  5, 63
        };
    public:
        static ulong flipBit(ulong mask, short bitToFlip);
        static bool  isEmpty(ulong mask);
        static short countBits(ulong board);
        static int bitScanForward(ulong board);
    };
}

#endif //MARLIN_BITOPS_H
