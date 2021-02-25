//
// Created by nibor on 24/02/2021.
//

#include "gtest/gtest.h"

TEST(ExampleTests, testing) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
