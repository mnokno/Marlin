//
// Created by Jakub Nowacki on 20/11/2022.
//

#include <list>
#include "gtest/gtest.h"
#include "types.h"
#include "prng.h"

using namespace engine;

class PRNGTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        this->prng = *new PRNG();
    }

    PRNG prng;
};

TEST_F(PRNGTest, Randomness){
    std::list<ulong> numbers;
    // good enough, the sequence does not loop for a long time
    for (int i = 0; i < 1000; i++){
        ulong number = prng.nextUlong();
        ASSERT_EQ(false, (std::find(numbers.begin(), numbers.end(), number) != numbers.end()));
        numbers.push_back(number);
    }
}