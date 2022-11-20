//
// Created by kubaa on 20/11/2022.
//

#include "bitops.h"

namespace engine{

    ulong BitOps::flipBit(ulong mask, short bitToFlip) {
        return (ulong)(mask ^ ((ulong)1 << (ulong)bitToFlip));
    }

    bool BitOps::isEmpty(ulong mask) {
        return mask == 0;
    }

    short BitOps::countBits(ulong board) {
        short count = 0;
        while (board != (ulong)0)
        {
            count++;
            board &= board - (ulong)1; // reset least significant one bit
        }
        return count;
    }
}