#include "Application.h"
#include "Log.h"

namespace ee
{
	Application::Application()
	{
		InitializeCore();
	}

	Application::~Application()
	{
		ShutdownCore();
	}

	void Application::Run()
	{
		while (true);
	}
}