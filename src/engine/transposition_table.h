//
// Created by Jakub Nowacki on 22/11/2022.
//

#ifndef MARLIN_TRANSPOSITION_TABLE_H
#define MARLIN_TRANSPOSITION_TABLE_H

#include "types.h"

namespace engine {

    struct TTEntry {
    public:
        [[nodiscard]] ulong getHash() const {return this->hash;}
        [[nodiscard]] byte getDepthFromRoot() const {return this->depthFromRoot;}
        [[nodiscard]] short getEval() const {return this->eval;}
        [[nodiscard]] byte getMove() const {return this->move;}
        [[nodiscard]] NodeType getNodeType() const {return this->nodeType;}
    private:
        friend class TranspositionTable;
        ulong hash;
        byte depthFromRoot;
        short eval;
        byte move;
        NodeType nodeType;
    };

    class TranspositionTable {
    public:
        [[nodiscard]] explicit TranspositionTable(uint tableSizeInEntries);
        [[nodiscard]] static int calculateTableCapacity(int maxTableSizeInMb);
        [[nodiscard]] TTEntry probe(ulong hash, int depth, bool& found);
        [[nodiscard]] TTEntry probe(ulong hash, int depth, bool& found, int alpha, int beta);
        void save(ulong hash, byte depth, short eval);
        void save(ulong hash, byte depth, short eval, byte move, NodeType nodeType);
        void clear();
        TTEntry* table;
    private:
        [[nodiscard]] uint getHashIndex(ulong hash) const;
        uint tableSize;
        int writes;
        int reads;
        int hits;
    };

} // engine

#endif //MARLIN_TRANSPOSITION_TABLE_H
