//
// Created by Jakub Nowacki on 20/11/2022.
//

#include "gtest/gtest.h"
#include "position.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

using namespace engine;

class PositionTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }
};

TEST_F(PositionTest, YellowWins) {
    Position position = *new Position();
    position.makeMove(0);
    position.makeMove(7);
    position.makeMove(1);
    position.makeMove(8);
    position.makeMove(2);
    position.makeMove(9);
    position.makeMove(3);
    ASSERT_EQ(GameState::YELLOW_WON, position.getGameState());
}

TEST_F(PositionTest, RedWins) {
    Position position = *new Position();
    position.makeMove(0);
    position.makeMove(7);
    position.makeMove(1);
    position.makeMove(8);
    position.makeMove(2);
    position.makeMove(9);
    position.makeMove(14);
    position.makeMove(3);
    position.makeMove(21);
    position.makeMove(10);
    ASSERT_EQ(GameState::RED_WON, position.getGameState());
}

TEST_F(PositionTest, Draw){
    Position position = *new Position();

    for (int i = 0; i < 6; i++){
        position.makeMove(position.convertFileToMove(0));
        position.makeMove(position.convertFileToMove(5));
        position.makeMove(position.convertFileToMove(1));
        position.makeMove(position.convertFileToMove(4));
        position.makeMove(position.convertFileToMove(2));
        position.makeMove(position.convertFileToMove(3));
        position.makeMove(position.convertFileToMove(6));
    }

    ASSERT_EQ(GameState::DRAW, position.getGameState());
}

TEST_F(PositionTest, ConvertFileToMove) {
    Position position = *new Position();
    position.makeMove(position.convertFileToMove(0));
    position.makeMove(position.convertFileToMove(0));
    position.makeMove(position.convertFileToMove(1));
    position.makeMove(position.convertFileToMove(1));
    position.makeMove(position.convertFileToMove(2));
    position.makeMove(position.convertFileToMove(2));
    position.makeMove(position.convertFileToMove(3));
    ASSERT_EQ(GameState::YELLOW_WON, position.getGameState());
}

TEST_F(PositionTest, UnMakeMoves){
    Position position = *new Position();
    GameState gameState = position.getGameState();
    ulong yellow = position.getPosition(0);
    ulong red = position.getPosition(1);
    Player player = position.getPlayerToMove();
    ulong hash = position.getHash();
    short moveCount = position.getMoveCount();

    // plays the moves
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 6; j++){
            position.makeMove(position.convertFileToMove(j));
        }
    }

    // un-plays the moves
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 6; j++){
            position.unMakeMove();
        }
    }

    ASSERT_EQ(gameState, position.getGameState());
    ASSERT_EQ(yellow, position.getPosition(0));
    ASSERT_EQ(red, position.getPosition(1));
    ASSERT_EQ(player, position.getPlayerToMove());
    ASSERT_EQ(hash, position.getHash());
    ASSERT_EQ(moveCount, position.getMoveCount());
}