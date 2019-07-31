#pragma once

#include <functional>

#include "Engine/Core/Core.h"
#include "Engine/Core/Events/Event.h"

namespace ee
{
	
	struct WindowProps
	{
		std::string Title;
		euint Width;
		euint Height;

		explicit WindowProps(const std::string& title = "Sravan - Engine", const euint width = 1280, const euint height = 720)
			: Title(title), Width(width), Height(height)
		{
			
		}

	};

	class Window
	{
		
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		[[nodiscard]] virtual euint GetWidth() const = 0;
		[[nodiscard]] virtual euint GetHeight() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

		virtual void OnUpdate() = 0;

	};

}