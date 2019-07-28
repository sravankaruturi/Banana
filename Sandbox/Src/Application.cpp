#include "Engine.h"
#include <gtest/gtest.h>

class Sandbox : public ee::Application
{
	
public:
	Sandbox()
	{
		EE_TRACE("Hello!");
	}

};

ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}

int main()
{

	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();

	ee::Application* app = ee::CreateApplication();
	app->Run();
	delete app;

}


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

