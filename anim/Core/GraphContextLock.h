#pragma once

namespace anim
{
	template<typename T>
	class GraphContextLock
	{
	public:
		GraphContextLock(T *context, CRITICAL_SECTION &lock);
		GraphContextLock(GraphContextLock<T> &&rhs);
		~GraphContextLock();

		T *GetContext() const;

	private:
		T *context;
		CRITICAL_SECTION *lock;
	};

	typedef GraphContextLock<ID3D11DeviceContext3> GraphContextLock3d;
	typedef GraphContextLock<ID2D1DeviceContext2> GraphContextLock2d;
}

template<typename T>
anim::GraphContextLock<T>::GraphContextLock(T *context, CRITICAL_SECTION &lock)
	: context(context)
	, lock(&lock)
{
}

template<typename T>
anim::GraphContextLock<T>::GraphContextLock(GraphContextLock<T> &&rhs)
	: context(rhs.context)
	, lock(rhs.lock)
{
	rhs.context = nullptr;
	rhs.lock = nullptr;
}

template<typename T>
anim::GraphContextLock<T>::~GraphContextLock()
{
	if (this->lock != nullptr)
	{
		::LeaveCriticalSection(this->lock);
	}
}

template<typename T>
T *anim::GraphContextLock<T>::GetContext() const
{
	return this->context;
}
