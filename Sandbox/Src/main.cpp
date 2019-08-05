#include "Tests/Tests.h"

int main()
{

	/*::testing::InitGoogleTest();
	RUN_ALL_TESTS();*/

	//char c;
	//std::cin >> c;

	ee::Application* app = ee::CreateApplication();
	EE_CORE_ASSERT(app, "Application is null");
	app->Run();
	delete app;

}