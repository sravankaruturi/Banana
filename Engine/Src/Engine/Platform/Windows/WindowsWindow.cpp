#pragma once

#include "WindowsWindow.h"
#include "Engine/Core/Log.h"

namespace ee
{

	Window * Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Title = props.Title;
		m_Width = props.Width;
		m_Height = props.Height;

		EE_CORE_INFO("Creating a window {0} ({1}, {2})", m_Title, m_Width, m_Height);
	}

	void WindowsWindow::ShutDown()
	{
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	= default;

	void WindowsWindow::OnUpdate()
	{
		
	}
}

