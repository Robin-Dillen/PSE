#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main() {
    testing::InitGoogleTest();
    std::cout << "Hello, World!" << std::endl;
    return RUN_ALL_TESTS();
}
