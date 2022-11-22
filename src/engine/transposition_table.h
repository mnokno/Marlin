//
// Created by kubaa on 22/11/2022.
//

#ifndef MARLIN_TRANSPOSITION_TABLE_H
#define MARLIN_TRANSPOSITION_TABLE_H

#include <cstdint>
#include "types.h"
#include "position.h"

namespace engine {

    struct TTEntry {
    public:
        ulong getHash() {return this->hash;}
        uint8_t getDepth() {return this->depth;}
        uint8_t getEval() {return this->eval;}
        uint8_t getMove() {return this->move;}
    private:
        friend class TranspositionTable;
        ulong hash;
        uint8_t depth;
        uint8_t eval;
        uint8_t move;
    };

    class TranspositionTable {
    public:
        TranspositionTable(int tableSize);
        TTEntry porbe(ulong , bool& found);
        void save(ulong hash, uint8_t depth, uint8_t eval, uint8_t move);
        TTEntry* table;
    private:
        int getHashIndex(ulong hash);
        int tableSize;
    };

} // engine

#endif //MARLIN_TRANSPOSITION_TABLE_H
