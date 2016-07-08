#include "pch.h"
#include "Core/Event.h"

anim::EventCookie anim::Event<void>::Add(FuncType &&func)
{
	return &*this->callbacks.emplace_after(this->callbacks.before_begin(), func);
}

void anim::Event<void>::Remove(EventCookie &cookie)
{
	if (cookie != NULL_EVENT_COOKIE)
	{
		*(FuncType *)cookie = FuncType();
		cookie = NULL_EVENT_COOKIE;
	}
}

void anim::Event<void>::Notify()
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
			(*i)();
		}
	}
}
