#include "Tests/SandboxTests.h"

int main()
{

	::testing::InitGoogleTest();
	RUN_ALL_TESTS();

	char c;
	std::cin >> c;

	ee::Application* app = ee::CreateApplication();
	app->Run();
	delete app;

}