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


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	};

}
