#pragma once

namespace anim
{
	typedef void *EventCookie;
	const EventCookie NULL_EVENT_COOKIE = nullptr;

	template<typename... Args>
	class Event
	{
	public:
		typedef std::function<void(Args...)> FuncType;

		EventCookie Add(FuncType &&func);
		void Remove(EventCookie cookie);
		void Notify(Args... args);

	private:
		std::forward_list<FuncType> callbacks;
	};

	typedef Event<const char *> ChangedEvent;
}

template<typename... Args>
anim::EventCookie anim::Event<Args...>::Add(FuncType &&func)
{
	return &*this->callbacks.emplace_after(this->callbacks.before_begin(), func);
}

template<typename... Args>
void anim::Event<Args...>::Remove(EventCookie cookie)
{
	*(FuncType *)cookie = FuncType();
}

template<typename... Args>
void anim::Event<Args...>::Notify(Args... args)
{
	for (auto prev = this->callbacks.before_begin(), i = this->callbacks.begin(); i != this->callbacks.end(); prev = i++)
	{
		if (*i == nullptr)
		{
			this->callbacks.erase_after(prev);
			i = prev;
		}
		else
		{
			(*i)(args...);
		}
	}
}
