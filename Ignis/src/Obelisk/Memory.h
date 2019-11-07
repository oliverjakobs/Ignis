#pragma once

#include <cstdlib>
#include <new>
#include <limits>

#include <iostream>

template <class Type>
struct ObeliskAllocator
{
	typedef Type value_type;

	ObeliskAllocator() = default;

	template <class Type>
	constexpr ObeliskAllocator(const ObeliskAllocator<Type>&) noexcept {}

	Type* allocate(std::size_t n)
	{
		std::cout << "Allocating " << n * sizeof(Type) << " byte(s)\n";

		if (n > std::numeric_limits<std::size_t>::max() / sizeof(Type))
			throw std::bad_alloc();

		if (auto p = static_cast<Type*>(std::malloc(n * sizeof(Type))))
			return p;

		throw std::bad_alloc();
	}

	void deallocate(Type* p, std::size_t) noexcept { std::free(p); }
};

template <class T, class U>
bool operator==(const ObeliskAllocator<T>&, const ObeliskAllocator<U>&) 
{ 
	return true;
}

template <class T, class U>
bool operator!=(const ObeliskAllocator<T>&, const ObeliskAllocator<U>&) 
{ 
	return false; 
}