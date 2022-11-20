//
// Created by kubaa on 20/11/2022.
//

#include <stack>
#include "gtest/gtest.h"
#include "zobrist_hashing.h"

using namespace engine;

class ZobristHashingTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        ZobristHashing::initHashes();
    }
};

TEST_F(ZobristHashingTest, Soundness){
    ulong hashOriginal = (ulong)1235961235;
    ulong hash = hashOriginal;

    stack<int> moves;
    stack<ulong> hashes;
    for (int i = 0; i < 1000; i++){
        for (int j = 0; j < 2; j++){
            moves.push(rand() % 42);
            hash = ZobristHashing::updateHash(hash, moves.top(), static_cast<Player>(j));
            hashes.push(hash);
        }
    }
    for (int i = 0; i < 1000; i++){
        for (int j = 1; j >= 0; j--){
            ASSERT_EQ(hash, hashes.top());
            hash = ZobristHashing::updateHash(hash, moves.top(), static_cast<Player>(j));
            hashes.pop();
            moves.pop();
        }
    }

    ASSERT_EQ(hashOriginal, hash);
}