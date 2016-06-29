#pragma once

namespace anim
{
	template<typename... Args>
	class Event
	{
	public:
		Event();
		~Event();

		typedef std::function<void(Args...)> FuncType;

		size_t Add(FuncType &&func);
		void Remove(size_t cookie);
		void Notify(Args... args);

	private:
		struct Entry
		{
			FuncType callback;

			Entry(FuncType &&callback)
				: callback(std::move(callback))
			{
			}
		};

		std::list<Entry> callbacks;
	};

	typedef Event<const char *> ChangedEvent;
}

template<typename... Args>
anim::Event<Args...>::Event()
{
}

template<typename... Args>
anim::Event<Args...>::~Event()
{
}

template<typename... Args>
size_t anim::Event<Args...>::Add(FuncType &&func)
{
	auto i = this->callbacks.emplace(this->callbacks.end(), std::move(func));
	return (size_t)&*i;
}

template<typename... Args>
void anim::Event<Args...>::Remove(size_t cookie)
{
	for (auto i = this->callbacks.begin(); i != this->callbacks.end(); i++)
	{
		if ((size_t)&*i == cookie)
		{
			this->callbacks.erase(i);
			break;
		}
	}
}

template<typename... Args>
void anim::Event<Args...>::Notify(Args... args)
{
	for (const Entry &entry : callbacks)
	{
		entry.callback(args...);
	}
}
