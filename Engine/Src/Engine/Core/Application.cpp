#include "eepch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/FrameBuffer.h"
#include <GLFW/glfw3.h>

#include <imgui.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

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

		ImGui::Begin("Renderer");
		auto& caps = re::RendererAPI::GetCapabilities();
		ImGui::Text("Vendor %s", caps.Vendor.c_str());
		ImGui::Text("Renderer %s", caps.Renderer.c_str());
		ImGui::Text("Version %s", caps.Version.c_str());
		ImGui::End();
		
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

		int width = e.GetWidth();
		int height = e.GetHeight();

		if ( width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}
		
		m_Minimized = false;

		EE_RENDER_2(width, height, { glViewport(0, 0, width, height); });

		auto& fbs = re::FrameBufferPool::GetGlobal()->GetAll();

		for(auto& fb: fbs)
		{
			fb->Resize(width, height);
		}
		
		return false;
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		EE_CORE_INFO("Window Closed");
		m_Running = false;
		return true;
	}

	std::string Application::OpenFile(const std::string& filter) const
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)m_Window->GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if ( GetOpenFileNameA(&ofn) == true)
		{
			return ofn.lpstrFile;
		}

		return std::string();
		
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
			if (!m_Minimized)
			{
				for (auto layer : m_LayerStack)
				{
					layer->OnUpdate();
				}

				Application* app = this;
				EE_RENDER_1(app, { app->RenderImGui(); });

				re::Renderer::GetInstance().WaitAndRender();
			}

			m_Window->OnUpdate();
		}

		OnShutDown();
	}
}
