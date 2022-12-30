//
// Created by Jakub Nowacki on 22/11/2022.
//

#include <iostream>
#include "constants.h"
#include "transposition_table.h"
#include "types.h"

namespace engine {

    /**
     * Constructor for TranspositionTable
     * Primes 999983 9999991
     *
     * @param tableSize Size of the table in entries
     */
    TranspositionTable::TranspositionTable(uint tableSizeInEntries) {
        std::cout << "Created new TT table Allocated entries: " << tableSizeInEntries << " In memory size: " << tableSizeInEntries * sizeof(TTEntry) / (1024 * 1024) << "MB" << std::endl;
        this->tableSize = (uint)tableSizeInEntries;
        this->table = new TTEntry[tableSize];
        this->writes = 0;
        this->reads = 0;
        this->hits = 0;
        clear();
    }

    /**
     * Calculates the max amount of entries that the translation table
     * can store while not exceeding the specified memory size;
     *
     * @param maxTableSizeInMb How much space will be allocated to the transportation table in MB
     * @return How many entries can be stored while remaining under specified memory size
     */
    int TranspositionTable::calculateTableCapacity(int maxTableSizeInMb) {
        return (int)((maxTableSizeInMb / sizeof(TTEntry)) * BYTES_IN_MB);
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
     * @param depthFromRoot Depth from root (start of the game)
     * @param found Reference to a boolean that will be set to true if the hash was found
     * @return The entry associated with the hash
     */
    TTEntry TranspositionTable::probe(ulong hash, int depthFromRoot, bool & found) {
        this->reads++;
        TTEntry entry = this->table[this->getHashIndex(hash)];
        if (entry.hash == hash && (entry.nodeType == EXACT && entry.depthFromRoot >= depthFromRoot)) {
            found = true;
            this->hits++;
            return entry;
        }
        else{
            found = false;
            return entry;
        }
    }

    /**
     * Tries to find an entry in the transposition table, should be used by alphaBeta NOT miniMax
     * @param hash Hash to probe
     * @param depthFromRoot Depth from root (start of the game)
     * @param found Reference to a boolean that will be set to true if the hash was found
     * @param alpha Alpha value
     * @param beta Beta value
     * @return The entry associated with the hash
     */
    TTEntry TranspositionTable::probe(ulong hash, int depthFromRoot, bool &found, int alpha, int beta) {
        this->reads++;
        TTEntry entry = this->table[this->getHashIndex(hash)];
        if (entry.hash == hash){
            if (entry.depthFromRoot >= depthFromRoot){
                // We have stored the exact evaluation for this position, so return it
                if (entry.nodeType == EXACT) {
                    found = true;
                    this->hits++;
                    return entry;
                }

                // We have stored the upper bound of the eval for this position. If it's less than alpha then we don't need to
                // search the moves in this position as they won't interest us; otherwise we will have to search to find the exact value
                if (entry.nodeType == UPPER_BOUND && entry.eval <= alpha) {
                    found = true;
                    this->hits++;
                    return entry;
                }

                // We have stored the lower bound of the eval for this position. Only return if it causes a beta cut-off.
                if (entry.nodeType == LOWER_BOUND && entry.eval >= beta) {
                    found = true;
                    this->hits++;
                    return entry;
                }
            }
        }

        found = false;
        return entry;
    }

    /**
     * Saves a value to the transposition table with node type EXACT, should only by used by miniMax NOT alphaBeta
     * @param hash Hash associated with the position
     * @param depthFromRoot Depth from root (start of the game)
     * @param eval Evaluation found from the search
     */
    void TranspositionTable::save(ulong hash, byte depthFromRoot, short eval) {
        writes++;
        this->table[this->getHashIndex(hash)].hash = hash;
        this->table[this->getHashIndex(hash)].depthFromRoot = depthFromRoot;
        this->table[this->getHashIndex(hash)].eval = eval;
        this->table[this->getHashIndex(hash)].nodeType = EXACT;
    }

    /**
     * Saves a value to the transposition table with given nodeType, should only by used by alphaBeta NOT miniMax
     * @param hash Hash associated with the position
     * @param depthFromRoot Depth from root (start of the game)
     * @param eval Evaluation found from the search
     * @param move Move that was chosen
     * @param nodeType Node type of the position
     */
     void TranspositionTable::save(ulong hash, byte depthFromRoot, short eval, byte move, NodeType nodeType) {
         writes++;
         this->table[this->getHashIndex(hash)].hash = hash;
         this->table[this->getHashIndex(hash)].depthFromRoot = depthFromRoot;
         this->table[this->getHashIndex(hash)].eval = eval;
         this->table[this->getHashIndex(hash)].move = move;
         this->table[this->getHashIndex(hash)].nodeType = nodeType;
    }

    /**
     * Clears the transposition table
     */
    void TranspositionTable::clear() {
        writes = 0;
        reads = 0;
        hits = 0;
        for (int i = 0; i < this->tableSize; i++) {
            this->table[i].hash = (ulong)0;
            this->table[i].depthFromRoot = (int)0;
            this->table[i].eval = (int)0;
            this->table[i].move = -1;
            this->table[i].nodeType = EMPTY;
        }
    }

} // engine