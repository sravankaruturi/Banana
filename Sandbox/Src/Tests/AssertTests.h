#pragma once

#include <gtest/gtest.h>
#include "Engine/Core/Log.h"

TEST(AssertTests, CoreAssertWorks)
{
	
	int a = 1;
	EE_CORE_ASSERT(a == 1, "Test Failed");

	int b = 2;
	EE_CORE_ASSERT(a != b, "Test Failed");

	int* c = nullptr;
	EE_CORE_ASSERT((!c), "Test Failed");

	// If they reach here without breaking, it is a success.
	EXPECT_EQ(a, 1);

}

TEST(AssertTests, AppAssertWorks)
{

	int a = 1;
	EE_ASSERT(a == 1, "Test Failed");

	int b = 2;
	EE_ASSERT(a != b, "Test Failed");

	int* c = nullptr;
	EE_ASSERT((!c), "Test Failed");

	// If they reach here without breaking, it is a success.
	EXPECT_EQ(a, 1);

}
