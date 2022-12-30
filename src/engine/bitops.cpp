//
// Created by kubaa on 20/11/2022.
//

#include "bitops.h"
#include "constants.h"

namespace engine{

    /**
     * Flips the given bit in the given number
     *
     * @param mask the number to flip the bit in
     * @param bit the bit to flip
     * @return the number with the bit flipped
     */
    ulong BitOps::flipBit(ulong mask, short bitToFlip) {
        return (ulong)(mask ^ ((ulong)1 << (ulong)bitToFlip));
    }

    /**
     * Checks if the given number is empty
     *
     * @param mask the number to check
     * @return true if the number is empty, false otherwise
     */
    bool BitOps::isEmpty(ulong mask) {
        return mask == 0;
    }

    /**
     * Counts the number of bits in the given number
     *
     * @param board the number to count the bits in
     * @return number of bits in the given number
     */
    short BitOps::countBits(ulong board) {
        short count = 0;
        while (board != (ulong)0)
        {
            count++;
            board &= board - (ulong)1; // reset least significant one bit
        }
        return count;
    }

    /**
     * Finds index of the first bit in the given number
     *
     * @param board the number to find the first bit in
     * @return index of the first bit in the given number
     */
    int BitOps::bitScanForward(ulong board) {
        return index64[((board ^ (board - 1)) * DEBRUIJN64) >> 58];
    }
}