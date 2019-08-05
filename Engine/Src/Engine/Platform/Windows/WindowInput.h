#pragma once
#include "Engine/Core/Input/Input.h"

namespace ee::in
{
	class WindowInput : public Input
	{

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
	
}
