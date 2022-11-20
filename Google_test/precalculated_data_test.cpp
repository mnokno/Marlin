//
// Created by kubaa on 20/11/2022.
//

#include <gtest/gtest.h>
#include <bitset>
#include "types.h"
#include "precalculated_data.h"

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
    std::string expected[4] = {"000000100000010000001000000100000010000001",
                            "100000010000001000000100000010000001000000",
                            "000000000000000000000000000000000001111111",
                            "111111100000000000000000000000000000000000"};
    for (int i = 0; i < 4; i++){
        ASSERT_EQ(expected[i], PrecalculatedData::formatUlong(PrecalculatedData::boarderMasks[i]));
    }
}

TEST_F(PrecalculatedDataTest, WiningLinesMasks){
    for (ulong mask : PrecalculatedData::winingLinesMasks[18]){
        //std::cout << PrecalculatedData::formatBoard(mask) << std::endl;
    }
}

