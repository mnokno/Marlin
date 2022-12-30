//
// Created by Jakub Nowacki on 20/11/2022.
//

#ifndef MARLIN_ZOBRIST_HASHING_H
#define MARLIN_ZOBRIST_HASHING_H

#include "types.h"
#include "position.h"

namespace engine {

    class ZobristHashing {
    private:
        // 0 = yellow, 1 = red
        inline static ulong hashes[2][42];
    public:
        static void initHashes();
        static ulong generateHash(Position *position);
        static ulong updateHash(ulong hash, int move, Player player);
    };

} // engine

#endif //MARLIN_ZOBRIST_HASHING_H
