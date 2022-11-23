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
    virtual void SetUp() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }

    static void calculateControlCase(int depth, BaseLevel baseLevel, Position &position, Search &search, list<int> &moves) {
        int gameLeafCount = 0;
        while (position.getGameState() == GameState::ON_GOING){
            int move = search.findBestMoveBaseTest(depth, baseLevel);
            position.makeMove(move);
            moves.push_back(move);
            gameLeafCount += search.getLeafNodes();
        }
        std::cout << formatPosition(position);
        std::cout << "^^^ CONTROL depth: " << depth << " leafs: " << gameLeafCount << " ^^^" << std::endl;
    }

    static void validateGame(int depth, BaseLevel baseLevel, Position &position, Search &search, list<int> &moves){
        // calculates control case
        Position cPosition = *new Position();
        Search cSearch = *new Search(cPosition);
        list<int> cMoves;
        SearchTest::calculateControlCase(depth, baseLevel, cPosition, cSearch, cMoves);

        // compares control case against test case
        for (int move : moves){
            ASSERT_EQ(move, cMoves.front());
            cMoves.pop_front();
        }
    }

    static void validateLevel(BaseLevel testLevel, BaseLevel validationLevel, int upToDepth){
        for (int depth = 1; depth <= upToDepth; depth++){

            int gameLeafCount = 0;
            Position tPosition = *new Position();
            Search tSearch = *new Search(tPosition);
            list<int> tMoves;

            while (tPosition.getGameState() == GameState::ON_GOING){
                int move = tSearch.findBestMoveBaseTest(depth, testLevel);
                tPosition.makeMove(move);
                tMoves.push_back(move);
                gameLeafCount += tSearch.getLeafNodes();
            }

            std::cout << formatPosition(tPosition);
            std::cout << "^^^ TESTCASE depth: " << depth << " leafs: " << gameLeafCount << " ^^^" << std::endl;

            validateGame(depth, validationLevel, tPosition, tSearch, tMoves);
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
    Search search = *new Search(position);
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
    validateLevel(engine::ALPHA_BETA, engine::MINI_MAX, 7);
}

TEST_F(SearchTest, MiniMaxTTTest) {
    validateLevel(engine::MINI_MAX_TT, engine::MINI_MAX, 7);
}

TEST_F(SearchTest, AlphaBetaTTTest) {
    validateLevel(engine::ALPHA_BETA_TT, engine::MINI_MAX, 7);
}

TEST_F(SearchTest, ChainTest) {
    validateLevel(engine::MINI_MAX_TT, MINI_MAX, 7);
    validateLevel(engine::ALPHA_BETA, engine::MINI_MAX_TT, 8);
    validateLevel(engine::ALPHA_BETA_TT,ALPHA_BETA, 9);
}
