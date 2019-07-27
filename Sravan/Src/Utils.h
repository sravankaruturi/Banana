#pragma once

#include <gtest/gtest.h>

class Utils
{
public:

	static int Add(int a, int b);


};

TEST(UtilsAddFunction, PositiveNumbers) {

	EXPECT_EQ(4, Utils::Add(2, 2));

}

TEST(UtilsAddFunction, NegativeNumbers) {

	EXPECT_EQ(0, Utils::Add(2, -2));

}