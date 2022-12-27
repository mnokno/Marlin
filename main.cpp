#include <iostream>

#include "position.h"
#include "search.h"
#include "transposition_table.h"
#include "precalculated_data.h"
#include "zobrist_hashing.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    engine::ZobristHashing::initHashes();
    engine::PrecalculatedData::init();
    engine::TranspositionTable tt = *new engine::TranspositionTable(engine::TranspositionTable::calculateTableCapacity(10000));
    engine::Position position = *new engine::Position();
    engine::Search search = *new engine::Search(position, tt);
    //std::cout << search.findBestMove(15) << std::endl;
    std::cout << search.findBestMoveABMT(12, 2) << std::endl;
    return 0;
}
