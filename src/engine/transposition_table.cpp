//
// Created by kubaa on 22/11/2022.
//

#include <cstdint>
#include <iostream>
#include "transposition_table.h"

namespace engine {
    TranspositionTable::TranspositionTable(int tableSize) {
        this->tableSize = tableSize;
        this->writes = 0;
        this->table = new TTEntry[tableSize];
        clear();
        for (int i = 0; i < tableSize; i++){
            if (table[i].getHash() != 0){
                std::cout << "Error" << std::endl;
            }
        }
    }

    int TranspositionTable::getHashIndex(ulong hash) {
        return (hash>>33) % (this->tableSize);
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

    void TranspositionTable::save(ulong hash, int depth, int eval, Position position) {
        //if (this->table[this->getHashIndex(hash)].depth == depth){
        //    if (this->table[this->getHashIndex(hash)].red == position.getPosition(Player::RED)){
        //        std::cout << "Error" << std::endl;
        //    }
        //    std::cout << "Unexpected overwrite" << std::endl;
        //}
        //else if (this->table[this->getHashIndex(hash)].hash !=0){
        //    std::cout << "Overwrite" << std::endl;
        //}
        writes++;
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depth = depth;
        this->table[this->getHashIndex(hash)].eval = eval;
        this->table[this->getHashIndex(hash)].yellow = position.getPosition(Player::YELLOW);
        this->table[this->getHashIndex(hash)].red = position.getPosition(Player::RED);
    }

    void TranspositionTable::clear() {
        for (int i = 0; i < this->tableSize; i++) {
            this->table[i].hash = (ulong)0;
            this->table[i].depth = (uint16_t)0;
            this->table[i].eval = (uint16_t)0;
            this->table[i].red = (ulong)0;
            this->table[i].yellow = (ulong)0;
        }
    }


} // engine