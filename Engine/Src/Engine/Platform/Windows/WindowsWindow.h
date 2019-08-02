#pragma once

#include "Engine/Core/Window.h"

#include <GLFW/glfw3.h>

namespace ee
{
	
	class WindowsWindow final : public Window
	{
		
	private:

		struct WindowData
		{
			std::string Title;
			euint Width;
			euint Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};


		WindowData m_WindowData;
		GLFWwindow* m_Window;

		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	public:

		explicit WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_WindowData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		[[nodiscard]] bool IsVSync() const override;

		[[nodiscard]] euint GetWidth() const override { return m_WindowData.Width; }
		[[nodiscard]] euint GetHeight() const override { return m_WindowData.Height; }
	};

}