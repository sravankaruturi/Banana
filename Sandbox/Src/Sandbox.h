#pragma once
#include "Engine.h"
#include "Engine/Core/Layers/ImGui/ImGuiLayer.h"

class GameLayer : public ee::Layer
{
	
public:
	GameLayer() = default;

	virtual ~GameLayer() = default;

	virtual void OnUpdate() override
	{
		ee::re::Renderer::Clear(0.2f, 0.3f, 0.8f, 1);
	}

};

class Sandbox : public ee::Application
{

public:
	Sandbox()
	{
		EE_TRACE("Hello!");
	}

	virtual void OnInit()
	{
		PushLayer(new GameLayer());
		PushOverlay(new ee::ImGuiLayer("ImGui"));
	}

};

inline ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}