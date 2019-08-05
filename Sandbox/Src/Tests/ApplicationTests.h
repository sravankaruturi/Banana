#pragma once

#include <gtest/gtest.h>
#include "Engine/Core/Application.h"

class ApplicationTest : public testing::Test
{

protected:
	ee::Application* a1{};

};

TEST_F(ApplicationTest, CheckIfMultipleApplicationsAllowedToInitialize)
{

	auto app1 = new ee::Application();
	auto app2 = new ee::Application();

	delete app1;
	delete app2;

	EXPECT_EQ(0, 0);

}