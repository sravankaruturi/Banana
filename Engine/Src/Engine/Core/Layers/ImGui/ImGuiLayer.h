#pragma once

#include "Engine/Core/Layers/Layer.h"

namespace ee{

	class ImGuiLayer : public Layer
	{

	private:

		float m_Time = 0.0f;

	public:

		ImGuiLayer();
		ImGuiLayer(const std::string& name);
		virtual ~ImGuiLayer();


		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

	};

}
