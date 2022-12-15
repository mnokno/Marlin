//
// Created by kubaa on 23/11/2022.
//

#include "gtest/gtest.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"
#include "position.h"
#include "search.h"
#include "bitops.h"
#include "constants.h"
#include "types.h"

using namespace engine;

class SearchTest : public ::testing::Test {

protected:
    void SetUp() override {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }

    static void calculateGameData(int depth, BaseLevel baseLevel, Position &position, Search &search, list<int> &moves){
        // calculates the data
        int gameLeafCount = 0;
        while (position.getGameState() == GameState::ON_GOING){
            int move = search.findBestMoveBaseTest(depth, baseLevel);
            position.makeMove(move);
            moves.push_back(move);
            gameLeafCount += search.getLeafNodes();
        }
        // logs results for manual inspection
        std::cout << formatPosition(position);
        std::cout << "^^^" << to_string(baseLevel) << "L depth: " << depth << " leafs: " << gameLeafCount << std::endl;
    }

    static void validateLevel(BaseLevel testLevel, BaseLevel validationLevel, int upToDepth){
        for (int i = 1; i <= upToDepth; i++){
            // calculates test data
            Position tPosition = *new Position();
            TranspositionTable ttt = *new TranspositionTable(TranspositionTable::calculateTableCapacity(2500));
            Search tSearch = *new Search(tPosition, ttt);
            list<int> tMoves = *new list<int>();
            calculateGameData(i, testLevel, tPosition, tSearch, tMoves);

            // calculates validation data
            Position vPosition = *new Position();
            TranspositionTable vtt = *new TranspositionTable(TranspositionTable::calculateTableCapacity(2500));
            Search vSearch = *new Search(vPosition, vtt);
            list<int> vMoves = *new list<int>();
            calculateGameData(i, validationLevel, vPosition, vSearch, vMoves);

            // compares results
            for (int move = 0; move < vMoves.size(); move++){
                ASSERT_EQ(tMoves.front(), vMoves.front());
                tMoves.pop_front();
                vMoves.pop_front();
            }

            delete ttt.table;
            delete vtt.table;
        }
    }

    static string formatPosition(Position position) {
        string formatted = "";
        for (int i = 0; i < 6; i++){
            string line = "";
            for (int j = 0; j < 7; j++){
                if ((position.getPosition(Player::YELLOW) & BitOps::flipBit(0, i * 7 + j)) != 0){
                    line += "X";
                }
                else if ((position.getPosition(Player::RED) & BitOps::flipBit(0, i * 7 + j)) != 0){
                    line += "O";
                }
                else{
                    line += "_";
                }
            }
            formatted = line + "\n" + formatted;
        }
        return formatted;
    }
};

TEST_F(SearchTest, MiniMaxTest) {
    Position position = *new Position();
    TranspositionTable tt = *new TranspositionTable(TEST_TT_SIZE);
    Search search = *new Search(position, tt);
    // from 1 to 6
    for (int depth = 1; depth < 7; depth++){
        search.findBestMoveBaseTest(depth, MINI_MAX);
        ASSERT_EQ(search.getLeafNodes(), pow(7,depth));
    }

    Position freshPosition = *new Position();
    ASSERT_EQ(position.getPlayerToMove(), freshPosition.getPlayerToMove());
    ASSERT_EQ(position.getGameState(), freshPosition.getGameState());
    ASSERT_EQ(position.getHash(), freshPosition.getHash());
    ASSERT_EQ(position.getMoveCount(), freshPosition.getMoveCount());
    ASSERT_EQ(position.getPosition(Player::RED), freshPosition.getPosition(Player::RED));
    ASSERT_EQ(position.getPosition(Player::YELLOW), freshPosition.getPosition(Player::YELLOW));
    for (int i = 0; i < 7; i++){
        ASSERT_EQ(position.getStackHeight(i), 0);
    }

}

TEST_F(SearchTest, AlphaBetaTest) {
    validateLevel(engine::MINI_MAX, engine::ALPHA_BETA, 7);
}


TEST_F(SearchTest, MultiThreadingTest) {
    validateLevel(engine::ALPHA_BETA, engine::ALPHA_BETA_MT, 8);
}

TEST_F(SearchTest, ProgressiveDeepningTest) {
    validateLevel(engine::ALPHA_BETA_MT, engine::ALPHA_BETA_PD, 12);
}

TEST_F(SearchTest, SpeedTest) {
    validateLevel(engine::ALPHA_BETA_PD, engine::ALPHA_BETA_PD, 8);
}