//
// Created by Jakub Nowacki on 21/11/2022.
//

#include <list>
#include "gtest/gtest.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"
#include "move_generator.h"

using namespace engine;

class MoveGeneratorTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }
};

TEST_F(MoveGeneratorTest, MoveGeneration){
    Position position = *new Position();

    for (int i = 0; i < 5; i++){
        list<int> moves = MoveGenerator::generateMoves(position);
        ASSERT_EQ(moves.size(), 7);
        for (int& move : moves){
            position.makeMove(move);
        }
    }
    for (int i = 0; i < 7; i++){
        list<int> moves = MoveGenerator::generateMoves(position);
        ASSERT_EQ(7 - i, moves.size());
        position.makeMove(moves.front());
    }

    list<int> moves = MoveGenerator::generateMoves(position);
    ASSERT_EQ(0, moves.size());
}
