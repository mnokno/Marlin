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
        int getDepth() {return this->depth;}
        int getEval() {return this->eval;}
        ulong getRed() {return this->red;}
        ulong getYellow() {return this->yellow;}
    private:
        friend class TranspositionTable;
        ulong hash;
        int depth;
        int eval;
        ulong red;
        ulong yellow;
    };

    class TranspositionTable {
    public:
        TranspositionTable(int tableSize);
        TTEntry porbe(ulong , bool& found);
        void save(ulong hash, int depth, int eval, Position position);
        void clear();
        TTEntry* table;
    private:
        int getHashIndex(ulong hash);
        int tableSize;
        int writes;
    };

} // engine

#endif //MARLIN_TRANSPOSITION_TABLE_H
