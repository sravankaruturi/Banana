#pragma once
#include "Engine.h"
#include "Engine/Core/Layers/ImGui/ImGuiLayer.h"

class EditorLayer : public ee::Layer
{

private:
	float m_ClearColour[4];
	
public:
	EditorLayer()
		: m_ClearColour{0.2f, 0.2f, 0.8f, 1.0f}
	{
		
	}

	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(ee::Event& event) override;
};


class Sandbox : public ee::Application
{

public:
	Sandbox();

	virtual void OnInit();
};

inline ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}