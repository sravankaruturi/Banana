#pragma once
#include "Engine.h"
#include "Engine/Core/Layers/ImGui/ImGuiLayer.h"

class Sandbox : public ee::Application
{

public:
	Sandbox()
	{
		EE_TRACE("Hello!");
	}

	virtual void OnInit()
	{
		PushOverlay(new ee::ImGuiLayer("ImGui"));
	}

};

inline ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}