#include "eepch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace ee
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{

		s_Instance = this;

		InitializeCore();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_Running = true;

		m_ImGuiLayer = new ImGuiLayer("Default ImGui");
		PushOverlay(m_ImGuiLayer);

		re::Renderer::Init();
	}


	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();
		for(auto it : m_LayerStack)
		{
			it->OnImGuiRender();
		}
		m_ImGuiLayer->End();
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.m_Handled)
				break;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		EE_CORE_INFO("Window Resize: {0}, {1}", e.GetWidth(), e.GetHeight());
		return false;
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		EE_CORE_INFO("Window Closed");
		m_Running = false;
		return true;
	}


	Application::~Application()
	{
		s_Instance = nullptr;
		ShutdownCore();
	}

	void Application::Run()
	{
		
		OnInit();

		while (m_Running)
		{
			for ( auto layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			Application* app = this;
			EE_RENDER_1(app, { app->RenderImGui(); });

			re::Renderer::GetInstance().WaitAndRender();

			m_Window->OnUpdate();
		}

		OnShutDown();
	}
}
