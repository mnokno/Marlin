//
// Created by Jakub Nowacki on 20/11/2022.
//

#include <gtest/gtest.h>
#include "types.h"
#include "precalculated_data.h"
#include "bitops.h"

using namespace engine;

class PrecalculatedDataTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine::PrecalculatedData::init();
    }

    // void TearDown() override {}
};

TEST_F(PrecalculatedDataTest, MovesMasks) {
    // board size 6x7 hence 6*7=42
    for (ulong i = 0; i < 42; i++){
        ASSERT_EQ(((ulong)1) << i, PrecalculatedData::moveMasks[i]);
    }
}

TEST_F(PrecalculatedDataTest, ColumnMasksA) {
    for (int i = 0; i < 7; i++){
        ulong mask = 0;
        for (int j = 0; j < 6; j++){
            mask |= ((ulong)1) << (ulong)(i + j * 7);
        }
        ASSERT_EQ(mask, PrecalculatedData::columMasks[i]);
    }
}

TEST_F(PrecalculatedDataTest, ColumnMasksB) {
    ulong total = 0;
    for (int i = 0; i < 7; i++){
        total ^= PrecalculatedData::columMasks[i];

    }
    ulong reminder = 0;
    for (ulong i = 42; i < 64; i++){
        reminder ^= (((ulong)1) << i);
    }
    ASSERT_EQ(total, ~reminder);
}

TEST_F(PrecalculatedDataTest, BorderMasks) {
    std::string expected[4] = {"0000000000000000000000000000100000010000001000000100000010000001",
                            "0000000000000000000000100000010000001000000100000010000001000000",
                            "0000000000000000000000000000000000000000000000000000000001111111",
                            "0000000000000000000000111111100000000000000000000000000000000000"};
    for (int i = 0; i < 4; i++){
        ASSERT_EQ(expected[i], PrecalculatedData::formatUlong(PrecalculatedData::boarderMasks[i]));
    }
}

TEST_F(PrecalculatedDataTest, WiningLinesMasks){
    for (int i = 0; i < 42; i++){
        for (ulong mask : PrecalculatedData::winingLinesMasks[i]){
            ASSERT_EQ(4, BitOps::countBits(mask)); // should have 4 bit
            ASSERT_NE(0, mask & (ulong)1 << i); // should overlap with center
            //std::cout << PrecalculatedData::formatBoard(mask) << std::endl;
        }
    }
}

TEST_F(PrecalculatedDataTest, AdjacencySquareMasks) {
    for (int i = 0; i < 42; i++){
        short flag = 0;
        for (ulong& boarder : PrecalculatedData::boarderMasks){
            if ((boarder & BitOps::flipBit(0, i)) != 0){
                flag++;
            }
        }
        switch (flag) {
            case 0:
                ASSERT_EQ(8, BitOps::countBits(PrecalculatedData::adjacencySquareMasks[i]));
                break;
            case 1:
                ASSERT_EQ(5, BitOps::countBits(PrecalculatedData::adjacencySquareMasks[i]));
                break;
            default: // case 2
                ASSERT_EQ(3, BitOps::countBits(PrecalculatedData::adjacencySquareMasks[i]));
        }
    }
}

TEST_F(PrecalculatedDataTest, PillarMasks) {
    for (int x = 0; x < 7; x++){
        for (int y = 0; y < 6; y++){
            int layerSize = (x == 0 || x == 6) ? 2 : 3;
            int height = 6 - y + (y == 0 ? 0 : 1);
            int expected = height * layerSize;
            ASSERT_EQ(expected, BitOps::countBits(PrecalculatedData::pillarMasks[x + 7 * y]));
        }
    }
}