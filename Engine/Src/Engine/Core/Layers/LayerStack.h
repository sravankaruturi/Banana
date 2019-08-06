#pragma once

#include "Engine/Core/Core.h"
#include "Layer.h"

namespace ee {

	class LayerStack
	{

	private:
		std::vector<Layer*> m_Layers;
		euint m_LayerInsertIndex = 0;

	public:

		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	};

}