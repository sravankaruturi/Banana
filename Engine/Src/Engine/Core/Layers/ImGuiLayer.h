#pragma once

#include "Engine/Core/Layers/Layer.h"

namespace ee{

	class ImGuiLayer : public Layer
	{

	public:

		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();


		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(ee::Event& inEvent) override;
	};

}
