//
// Created by kubaa on 20/11/2022.
//

#include "zobrist_hashing.h"
#include "types.h"
#include "prng.h"

namespace engine {

    void ZobristHashing::initHashes() {
        PRNG prng = *new PRNG();
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 42; j++) {
                hashes[i][j] = prng.nextUlong();
            }
        }
    }

    ulong ZobristHashing::generateHash(Position position) {
        //TODO
        return 0;
    }

    ulong ZobristHashing::updateHash(ulong hash, int move, Player player) {
        return hash ^ hashes[player][move];
    }
} // engine