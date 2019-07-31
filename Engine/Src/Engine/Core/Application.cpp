#include "Application.h"
#include "Log.h"

namespace ee
{
	Application::Application()
	{
		InitializeCore();
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
		ShutdownCore();
	}

	void Application::Run()
	{
		while (true)
		{
			m_Window->OnUpdate();
		}
	}
}