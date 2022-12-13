#include <iostream>
#include "search.h"
#include "position.h"
#include "constants.h"
#include "precalculated_data.h"
#include "zobrist_hashing.h"

using namespace engine;

int main() {
    std::cout << "Hello, World!" << std::endl;
    PrecalculatedData::init();
    ZobristHashing::initHashes();
    Position position = *new Position();
    TranspositionTable tt = *new TranspositionTable(TranspositionTable::calculateTableCapacity(5000));
    Search search = *new Search(position, tt);
    cout << search.findBestMoveABMT(16) << endl;
    //cout << search.findBestMoveBaseTest(12, BaseLevel::ALPHA_BETA) << endl;
    return 0;
}
