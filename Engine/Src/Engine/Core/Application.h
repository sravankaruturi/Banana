#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

namespace ee
{
	
	class Application
	{

	private:
		std::unique_ptr<Window> m_Window;

	private:

		virtual void OnInit() {}
		virtual void OnShutDown() {}
		virtual void OnUpdate() {}

	public:
		Application();

		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}