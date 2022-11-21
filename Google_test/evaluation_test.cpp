//
// Created by kubaa on 21/11/2022.
//

#include <istream>
#include "gtest/gtest.h"
#include "position.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"
#include "evaluation.h"
#include "constants.h"

using namespace engine;

class EvaluationTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
    }
};

TEST_F(EvaluationTest, YellowWins) {
    Position position = *new Position();
    position.makeMove(0);
    position.makeMove(7);
    position.makeMove(1);
    position.makeMove(8);
    position.makeMove(2);
    position.makeMove(9);
    position.makeMove(3);
    ASSERT_EQ(EVAL_INFINITY, Evaluation::eval(position));
}

TEST_F(EvaluationTest, RedWins) {
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
    ASSERT_EQ(-EVAL_INFINITY, Evaluation::eval(position));
}

TEST_F(EvaluationTest, PositiveEval){
    Position position = *new Position();
    position.makeMove(position.convertFileToMove(6));
    for (int i = 0; i < 3; i++){
        position.makeMove(position.convertFileToMove(i));
        position.makeMove(position.convertFileToMove(i));
    }
    // should be 3 in the current evaluation function version
    ASSERT_GT(Evaluation::eval(position), 0);
}

TEST_F(EvaluationTest, NegativeEval){
    Position position = *new Position();
    for (int i = 0; i < 3; i++){
        position.makeMove(position.convertFileToMove(i));
        position.makeMove(position.convertFileToMove(i));
    }
    // should be -2 in the current evaluation function version
    ASSERT_LT(Evaluation::eval(position), 0);
}

TEST_F(EvaluationTest, Draw){
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

    ASSERT_EQ(0, Evaluation::eval(position));
}