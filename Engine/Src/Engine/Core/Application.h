#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Core/Layers/LayerStack.h"
#include "Engine/Core/Events/ApplicationEvent.h"

namespace ee
{
	
	class Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;

		LayerStack m_LayerStack;

	protected:

		virtual void OnInit() {}
		virtual void OnShutDown() {}
		virtual void OnUpdate() {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowClosedEvent& e);

	public:
		Application();

		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}