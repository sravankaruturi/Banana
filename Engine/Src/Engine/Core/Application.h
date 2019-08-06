#pragma once
#include "Engine/Core/Core.h"

#include "Engine/Core/Window.h"
#include "Engine/Core/Layers/LayerStack.h"
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Layers/ImGui/ImGuiLayer.h"

namespace ee
{
	
	class Application
	{

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		static Application* s_Instance;

		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowClosedEvent& e);

	public:

		inline Window& GetWindow() { return *m_Window; }
		static inline Application& GetInstance() { return *s_Instance; }

		Application();

		virtual ~Application();

		void Run();

		virtual void OnInit() {}
		virtual void OnShutDown() {}
		virtual void OnUpdate() {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void RenderImGui();

	};

	Application* CreateApplication();

}
