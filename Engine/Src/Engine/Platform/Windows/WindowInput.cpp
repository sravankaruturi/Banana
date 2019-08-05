#include "eepch.h"
#include "WindowInput.h"
#include "Engine/Core/Application.h"
#include "WindowsWindow.h"


namespace ee::in
{

	bool WindowInput::IsKeyPressedImpl(int keycode)
	{
		auto& window = static_cast<WindowsWindow&>(Application::GetInstance().GetWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeWindow()), keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowInput::IsMouseButtonPressedImpl(int button)
	{
		auto& window = static_cast<WindowsWindow&>(Application::GetInstance().GetWindow());

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeWindow()), button);
		return state == GLFW_PRESS;
	}

	float WindowInput::GetMouseXImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::GetInstance().GetWindow());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		return (float)xpos;
	}

	float WindowInput::GetMouseYImpl()
	{
		auto& window = static_cast<WindowsWindow&>(Application::GetInstance().GetWindow());

		double xpos, ypos;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &xpos, &ypos);

		return (float)ypos;
	}
}
