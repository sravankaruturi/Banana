#include "eepch.h"
#include "ImGuiLayer.h"

namespace ee {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{

	}

	ImGuiLayer::ImGuiLayer(const std::string& name)
		: Layer(name)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{

	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
	}

	void ImGuiLayer::OnEvent(ee::Event& inEvent)
	{
		if (GetName() == "Second Layer" && inEvent.IsInCategory(EventCategoryKeyBoard))
		{
			inEvent.m_Handled = true;
		}
		if (GetName() == "Second Layer")
		{
			EE_INFO("{0}: {1}", GetName(), inEvent.ToString());
		}
		else if (GetName() == "ImGui Layer")
		{
			EE_WARN("{0}: {1}", GetName(), inEvent.ToString());
		}
	}
}
