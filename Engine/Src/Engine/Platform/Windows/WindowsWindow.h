#pragma once

#include "Engine/Core/Window.h"

namespace ee
{
	
	class WindowsWindow final : public Window
	{
		
	private:

		std::string m_Title;
		euint m_Width;
		euint m_Height;

		EventCallbackFn m_EventCallbackFn;

		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	public:

		explicit WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallbackFn = callback; }

		[[nodiscard]] euint GetWidth() const override { return m_Width; }
		[[nodiscard]] euint GetHeight() const override { return m_Height; }
	};

}