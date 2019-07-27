#pragma once

#include "Engine/Core/Core.h"

namespace ee
{
	
	class Application
	{
	private:

	public:
		Application();

		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}