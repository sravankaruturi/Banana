#pragma once

#include "Engine/Core/Core.h"
#include <functional>

namespace ee
{
	
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResize, WindowFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyBoard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
	};

}

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){ return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char * GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

namespace ee
{
	
	class Event
	{
		
		friend class EventDispatcher;

	protected:
		bool m_Handled = false;

	public:
		virtual ~Event() = default;

		[[nodiscard]] virtual EventType GetEventType() const = 0;
		[[nodiscard]] virtual const char * GetName() const = 0;
		[[nodiscard]] virtual int GetCategoryFlags() const = 0;

		[[nodiscard]] bool IsInCategory(const EventCategory category) const
		{
			return GetCategoryFlags() && category;
		}

	};

	class EventDispatcher
	{
		
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	private:
		Event& m_Event;

	public:
		explicit EventDispatcher(Event& event)
			: m_Event(event)
		{
			
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			
			if ( m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)& m_Event);
				return true;
			}

			return false;

		}

	};

}