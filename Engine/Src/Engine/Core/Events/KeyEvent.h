#pragma once

#include "Event.h"

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

		EVENT_CLASS_TYPE(KeyPressed);
	};

	class KeyReleasedEvent : public KeyEvent
	{

	public:
		KeyReleasedEvent(const int keyCode)
			: KeyEvent(keyCode)
		{
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

}
