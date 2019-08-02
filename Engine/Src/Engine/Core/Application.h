#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Core/Events/ApplicationEvent.h"

namespace ee
{
	
	class Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;

	private:

		virtual void OnInit() {}
		virtual void OnShutDown() {}
		virtual void OnUpdate() {}

		virtual void OnEvent(Event& event);

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowClosedEvent& e);

	public:
		Application();

		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}