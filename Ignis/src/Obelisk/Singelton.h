#pragma once

#include <mutex>
#include <memory>

template<class T> class Singleton
{
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static std::unique_ptr<T> instance;
	static std::once_flag onceFlag;
public:
	Singleton() = default;

	static T& Singleton::Get()
	{
		std::call_once(Singleton::onceFlag, []()
		{
			instance.reset(new T());
		});

		return *(instance.get());
	}
};

template<class T> std::unique_ptr<T> Singleton<T>::instance;
template<class T> std::once_flag Singleton<T>::onceFlag;