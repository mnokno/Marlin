//
// Created by kubaa on 20/11/2022.
//

#include <gtest/gtest.h>
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

TEST_F(PrecalculatedDataTest, Moves) {
    // board size 6x7 hence 6*7=42
    for (ulong i = 0; i < 42; i++){
        ASSERT_EQ(((ulong)1) << i, PrecalculatedData::moveMasks[i]);
    }
}
