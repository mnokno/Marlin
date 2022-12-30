//
// Created by kubaa on 20/11/2022.
//

#include "zobrist_hashing.h"
#include "types.h"
#include "prng.h"
#include "bitops.h"

namespace engine {

    /**
     * Initializes the random numbers used for hashing.
     */
    void ZobristHashing::initHashes() {
        PRNG prng = *new PRNG();
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 42; j++) {
                hashes[i][j] = prng.nextUlong();
            }
        }
    }

    /**
     * Calculates hash for a new position that can be later updated move by move.
     *
     * @param position the position to calculate the hash for.
     * @return the hash of the position.
     */
    ulong ZobristHashing::generateHash(Position *position) {
        ulong hash = 0;
        for (int i = 0; i < 2; i++){
            for (short j = 0; j < 42; j++){
                // if the bit (j) is present in the position
                if ((position->getPosition(i) & BitOps::flipBit(0, j)) != 0){
                    hash ^= hashes[i][j];
                }
            }
        }
        return hash;
    }

    /**
     * Updates hash based on the move that was made.
     *
     * @param hash hash to update.
     * @param move played move.
     * @param player player who played the move.
     * @return updated hash.
     */
    ulong ZobristHashing::updateHash(ulong hash, int move, Player player) {
        return hash ^ hashes[player][move];
    }
} // engine