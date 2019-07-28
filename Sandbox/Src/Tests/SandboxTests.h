#pragma once

#include <gtest/gtest.h>
#include "Sandbox.h"

class SandboxTest : public testing::Test
{

protected:
	ee::Application* a1{};

};

TEST_F(SandboxTest, CheckIfLoggerInitialized)
{

	a1 = new Sandbox();

	EXPECT_TRUE(NULL != ee::Log::GetClientLogger());

	delete a1;

	EXPECT_TRUE(NULL != ee::Log::GetClientLogger());

}

