#pragma once
#include "Engine.h"
#include "Engine/Core/Layers/ImGuiLayer.h"

class Sandbox : public ee::Application
{

public:
	Sandbox()
	{
		EE_TRACE("Hello!");
	}

	virtual void OnInit()
	{
		PushLayer(new ee::ImGuiLayer());
		PushOverlay(new ee::ImGuiLayer("Second Layer"));
	}

};

inline ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}