//
// Created by kubaa on 20/11/2022.
//

#ifndef MARLIN_BITOPS_H
#define MARLIN_BITOPS_H

#include "types.h"

namespace engine{
    class BitOps {
    public:
        static ulong flipBit(ulong mask, short bitToFlip);
        static bool  isEmpty(ulong mask);
        static short countBits(ulong board);
    };
}

#endif //MARLIN_BITOPS_H
