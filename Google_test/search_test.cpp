//
// Created by kubaa on 23/11/2022.
//

#include "gtest/gtest.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"
#include "position.h"
#include "search.h"

using namespace engine;

class SearchTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }
};

TEST_F(SearchTest, MiniMaxTest) {
    Position position = Position();
    Search search = Search(position);
    // from 1 to 6
    for (int depth = 1; depth < 7; depth++){
        search.findBestMoveBaseTest(depth, 0);
        ASSERT_EQ(search.getLeafNodes(), pow(7,depth));
    }
}