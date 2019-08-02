#pragma once

#include "Event.h"

#include <sstream>

namespace ee
{
	
	class MouseMovedEvent final : public Event
	{

	private:
		float m_MouseX;
		float m_MouseY;

	public:
		MouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y)
		{
			
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}
		
		[[nodiscard]] float GetX() const { return m_MouseX; }
		[[nodiscard]] float GetY() const { return m_MouseX; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY( EventCategoryMouse | EventCategoryInput)

	};

	class MouseScrolledEvent : public Event
	{
		
	private:
		float m_XOffset;
		float m_YOffset;

	public:

		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{

		}

		[[nodiscard]] float GetXOffset() const	{ return m_XOffset; }
		[[nodiscard]] float GetYOffset() const { return m_YOffset;	}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);


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

	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button)
		{
			
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)

	};

	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{	
	public:
		explicit MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button)
		{
			
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedPressed: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};



}
