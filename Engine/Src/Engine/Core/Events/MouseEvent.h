#pragma once

#include "Event.h"

namespace ee
{
	
	class MouseMovedEvent final : public Event
	{

	private:
		float m_MouseX;
		float m_MouseY;
		float m_MouseDx;
		float m_MouseDy;

	public:
		MouseMovedEvent(const float x, const float y, const float dx, const float dy)
			: m_MouseX(x), m_MouseY(y), m_MouseDx(dx), m_MouseDy(dy)
		{
			
		}
		
		[[nodiscard]] float GetX() const { return m_MouseX; }
		[[nodiscard]] float GetY() const { return m_MouseX; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput)

	};

	class MouseButtonEvent : public Event
	{
	
	protected:
		int m_Button;

		explicit MouseButtonEvent(const int button)
			: m_Button(button){}

	public:
		[[nodiscard]] int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		
	};

	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
		
	private:
		int m_RepeatCount;

	public:
		MouseButtonPressedEvent(const int button, const int repeatCount)
			: MouseButtonEvent(button), m_RepeatCount(repeatCount)
		{
			
		}

		[[nodiscard]] int GetRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(MouseButtonPressed)

	};

	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{	
	public:
		explicit MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button)
		{
			
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};



}
