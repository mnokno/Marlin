//
// Created by Jakub Nowacki on 20/11/2022.
//

#include "gtest/gtest.h"
#include "bitops.h"

using namespace engine;

TEST(BitOpsTest, IsEmpty){
    ASSERT_EQ(true, BitOps::isEmpty((ulong)0));
    ulong num = 0;
    for (int i = 0; i < 1000; i++){
        if (num == 0){
            ASSERT_EQ(true, BitOps::isEmpty((ulong)0));
        }
        else{
            ASSERT_EQ(false, BitOps::isEmpty((ulong)num));
        }
        num ^= ((ulong)1 << (rand() % 64));
    }
}

TEST(BitOpsTest, FlipBit){
    ulong num = 0;
    for (int i = 0; i < 1000; i++){
        int randomBit = rand() % 64;
        ulong tmp = BitOps::flipBit(num, randomBit);
        ASSERT_EQ(tmp, num ^ ((ulong)1 << randomBit));
        num = randomBit;
    }
}

TEST(BitOpsTest, CountBits){
    ulong num = 0;
    ASSERT_EQ(0, BitOps::countBits(num));
    for (int i = 0; i < 64; i++){
        num ^= (ulong)1 << i;
        ASSERT_EQ(i + 1, BitOps::countBits(num));
    }
}

TEST(BitOpsTest, GetFirstBit){
    for (int i = 0; i < 64; i++){
        ulong mask = (ulong)1 << (ulong)i;
        for (int j = i + 1; j < 64; j++){
            if (rand() % 1000 < 500){
                mask ^= (ulong)1 << (ulong)j;
            }
            ASSERT_EQ(i, BitOps::bitScanForward(mask));
        }

        while (!BitOps::isEmpty(mask)){
            int firstBit = BitOps::bitScanForward(mask);
            mask ^= (ulong)1 << (ulong)firstBit;

        }
        ASSERT_EQ(0, mask);
    }
}
