//
// Created by kubaa on 22/11/2022.
//

#include "transposition_table.h"
#include "types.h"

namespace engine {

    TranspositionTable::TranspositionTable(uint tableSize) {
        this->tableSize = (uint)tableSize;
        this->writes = 0;
        this->table = new TTEntry[tableSize];
        clear();
    }

    uint TranspositionTable::getHashIndex(ulong hash) const {
        return (uint)(hash>>32) % (this->tableSize);
    }

    TTEntry TranspositionTable::porbe(ulong hash, bool& found) {
        if (this->table[this->getHashIndex(hash)].hash == hash) {
            found = true;
            return this->table[this->getHashIndex(hash)];
        }
        else{
            found = false;
            return this->table[this->getHashIndex(hash)];
        }
    }

    void TranspositionTable::save(ulong hash, int depth, int eval) {
        writes++;
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depth = depth;
        this->table[this->getHashIndex(hash)].eval = eval;
    }

    void TranspositionTable::clear() const {
        for (int i = 0; i < this->tableSize; i++) {
            this->table[i].hash = (ulong)0;
            this->table[i].depth = (int)0;
            this->table[i].eval = (int)0;
        }
    }

} // engine