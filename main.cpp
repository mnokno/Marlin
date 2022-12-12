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
    TranspositionTable tt = *new TranspositionTable(TEST_TT_SIZE);
    Search search = *new Search(position, tt);
    //cout << search.findBestMoveMMMT(8) << endl;
    cout << search.findBestMoveBaseTest(8, BaseLevel::MINI_MAX) << endl;
    return 0;
}
