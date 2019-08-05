#pragma once

namespace ee::in
{

	class Input
	{

	private:
		static Input* s_Instance;

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressed(button); }
		inline static float GetMouseX() { return s_Instance->GetMouseX(); }
		inline static float GetMouseY() { return s_Instance->GetMouseY(); }

	// The implementations and the actual functions are different because static functions and virtual functions don't mix?
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	public:

	};
}
