//
// Created by kubaa on 22/11/2022.
//

#include <cstdint>
#include "transposition_table.h"
#include "constants.h"

namespace engine {
    TranspositionTable::TranspositionTable(int tableSize) {
        this->table = new TTEntry[tableSize];
        for (int i = 0; i < tableSize; i++) {
            this->table[i].hash = (ulong)0;
            this->table[i].depth = (uint16_t)0;
            this->table[i].eval = (uint16_t)0;
            this->table[i].move = NO_MOVE;
        }
    }

    int TranspositionTable::getHashIndex(ulong hash) {
        return hash % this->tableSize;
    }

    TTEntry TranspositionTable::porbe(ulong hash, bool& found) {
        if (this->table[this->getHashIndex(hash)].hash == hash) {
            found = true;
            return this->table[this->getHashIndex(hash)];
        }
        else{
            found = false;
            return {};
        }
    }

    void TranspositionTable::save(ulong hash, uint8_t depth, uint8_t eval, uint8_t move) {
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depth = depth;
        this->table[this->getHashIndex(hash)].eval = eval;
        this->table[this->getHashIndex(hash)].move = move;
    }


} // engine