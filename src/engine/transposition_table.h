//
// Created by kubaa on 22/11/2022.
//

#ifndef MARLIN_TRANSPOSITION_TABLE_H
#define MARLIN_TRANSPOSITION_TABLE_H

#include "types.h"

namespace engine {

    struct TTEntry {
    public:
        [[nodiscard]] ulong getHash() const {return this->hash;}
        [[nodiscard]] int getDepth() const {return this->depth;}
        [[nodiscard]] int getEval() const {return this->eval;}
    private:
        friend class TranspositionTable;
        ulong hash;
        int depth;
        int eval;
    };

    class TranspositionTable {
    public:
        [[nodiscard]] explicit TranspositionTable(uint tableSize);
        [[nodiscard]] TTEntry porbe(ulong , bool& found);
        void save(ulong hash, int depth, int eval);
        void clear() const;
        TTEntry* table;
    private:
        [[nodiscard]] uint getHashIndex(ulong hash) const;
        uint tableSize;
        int writes;
    };

} // engine

#endif //MARLIN_TRANSPOSITION_TABLE_H
