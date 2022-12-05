//
// Created by kubaa on 22/11/2022.
//

#include "transposition_table.h"
#include "types.h"

namespace engine {

    /**
     * Constructor for TranspositionTable
     * Primes 999983 9999991
     *
     * @param tableSize Size of the table in entries
     */
    TranspositionTable::TranspositionTable(uint tableSize) {
        this->tableSize = (uint)tableSize;
        this->writes = 0;
        this->table = new TTEntry[tableSize];
        clear();
    }

    /**
     * Converts hash to table index
     *
     * @param hash Hash of the position
     * @return returns the index of the hash in the table
     */
    uint TranspositionTable::getHashIndex(ulong hash) const {
        return (uint)(hash>>32) % (this->tableSize);
    }

    /**
     * Tries to find an entry in the transposition table, should be used by miniMax NOT alphaBeta
     * @param hash Hash to probe
     * @param found Reference to a boolean that will be set to true if the hash was found
     * @return The entry associated with the hash
     */
    TTEntry TranspositionTable::probe(ulong hash, bool& found) {
        if (this->table[this->getHashIndex(hash)].hash == hash && this->table[this->getHashIndex(hash)].nodeType == EXACT) {
            found = true;
            return this->table[this->getHashIndex(hash)];
        }
        else{
            found = false;
            return this->table[this->getHashIndex(hash)];
        }
    }

    /**
     * Tries to find an entry in the transposition table, should be used by alphaBeta NOT miniMax
     * @param hash Hash to probe
     * @param found Reference to a boolean that will be set to true if the hash was found
     * @param alpha Alpha value
     * @param beta Beta value
     * @return The entry associated with the hash
     */
    TTEntry TranspositionTable::probe(ulong hash, bool &found, int alpha, int beta) {
        if (this->table[this->getHashIndex(hash)].hash == hash){
            // We have stored the exact evaluation for this position, so return it
            if (this->table[this->getHashIndex(hash)].nodeType == EXACT) {
                found = true;
                return this->table[this->getHashIndex(hash)];
            }

            // We have stored the upper bound of the eval for this position. If it's less than alpha then we don't need to
            // search the moves in this position as they won't interest us; otherwise we will have to search to find the exact value
            if (this->table[this->getHashIndex(hash)].nodeType == UPPER_BOUND &&
                        this->table[this->getHashIndex(hash)].eval <= alpha) {
                found = true;
                return this->table[this->getHashIndex(hash)];
            }

            // We have stored the lower bound of the eval for this position. Only return if it causes a beta cut-off.
            if (this->table[this->getHashIndex(hash)].nodeType == LOWER_BOUND &&
                     this->table[this->getHashIndex(hash)].eval >= beta) {
                found = true;
                return this->table[this->getHashIndex(hash)];
            }
        }
        found = false;
        return this->table[this->getHashIndex(hash)];
    }

    /**
     * Saves a value to the transposition table with node type EXACT, should only by used by miniMax NOT alphaBeta
     * @param hash Hash associated with the position
     * @param depth Depth of the search with was conducted to find the eval
     * @param eval Evaluation found from the search
     */
    void TranspositionTable::save(ulong hash, int depth, int eval) {
        writes++;
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depth = depth;
        this->table[this->getHashIndex(hash)].eval = eval;
        this->table[this->getHashIndex(hash)].nodeType = EXACT;
    }

    /**
     * Saves a value to the transposition table with given nodeType, should only by used by alphaBeta NOT miniMax
     * @param hash Hash associated with the position
     * @param depth Depth of the search with was conducted to find the eval
     * @param eval Evaluation found from the search
     * @param nodeType Node type of the position
     */
     void TranspositionTable::save(ulong hash, int depth, int eval, int move, NodeType nodeType) {
        writes++;
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depth = depth;
        this->table[this->getHashIndex(hash)].eval = eval;
        this->table[this->getHashIndex(hash)].move = move;
        this->table[this->getHashIndex(hash)].nodeType = nodeType;
    }

    /**
     * Clears the transposition table
     */
    void TranspositionTable::clear() const {
        for (int i = 0; i < this->tableSize; i++) {
            this->table[i].hash = (ulong)0;
            this->table[i].depth = (int)0;
            this->table[i].eval = (int)0;
        }
    }

} // engine