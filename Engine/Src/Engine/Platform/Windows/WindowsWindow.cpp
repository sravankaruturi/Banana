#include "eepch.h"
#include "WindowsWindow.h"

#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"

namespace ee
{

	static void glfw_error_callback(int error, const char * description)
	{
		EE_CORE_ERROR("Glfw Error: ({0}) : {1}", error, description);
	}

	static bool glfw_initialized = false;

	Window * Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_WindowData.Title = props.Title;
		m_WindowData.Width = props.Width;
		m_WindowData.Height = props.Height;

		EE_CORE_INFO("Creating a window {0} ({1}, {2})", m_WindowData.Title, m_WindowData.Width, m_WindowData.Height);

		if ( !glfw_initialized)
		{
			const auto success = glfwInit();
			EE_CORE_ASSERT(success, "Could not initialte Glfw!");

			glfwSetErrorCallback(glfw_error_callback);
			glfw_initialized = true;
		}

		m_Window = glfwCreateWindow(
			static_cast<int>(m_WindowData.Width),static_cast<int>(m_WindowData.Height),
			m_WindowData.Title.c_str(),nullptr, nullptr
			);

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow * window, int width, int height)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowResizeEvent event(static_cast<euint>(width), static_cast<euint>(height));
			data.EventCallback(event);

			data.Height = height;
			data.Width = width;
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow * window)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowClosedEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow * window, int key, int scancode, int action, int mods){

			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			default: EE_ASSERT(false, "Should not reach here.");
			}
			}
		);

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow * window, int button, int action, int mods)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			default: EE_ASSERT(false, "Should not reach here.");
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
			data.EventCallback(event);
		});

	}

	void WindowsWindow::ShutDown()
	{
		if ( nullptr != m_Window)
		{
			glfwDestroyWindow(m_Window);
		}
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_WindowData.VSync = enabled;
			
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_WindowData.VSync;
	}
}

