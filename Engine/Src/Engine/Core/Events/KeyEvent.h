#pragma once

#include "Event.h"
#include "Engine/Core/Input/KeyCodes.h"

#include <sstream>

namespace ee
{
	
	class KeyEvent : public Event
	{
		
	protected:
		int m_KeyCode;

		explicit KeyEvent(int keyCode)
			: m_KeyCode(keyCode)
		{
			
		}

	public:
		[[nodiscard]] int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput)

	};

	class KeyPressedEvent : public KeyEvent
	{
		
	private:
		int m_RepeatCount;

	public:
		KeyPressedEvent(const int keyCode, const int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount)
		{
		}

		[[nodiscard]] int GetRepeatCount() const { return m_RepeatCount; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class KeyReleasedEvent : public KeyEvent
	{

	public:
		KeyReleasedEvent(const int keyCode)
			: KeyEvent(keyCode)
		{
		}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent: public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}
