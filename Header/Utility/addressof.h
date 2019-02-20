#pragma once

// STL addressof tool

#include <memory>

namespace MSTD {
	template<typename T>
	T* addressof(T &val)
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(
				reinterpret_cast<const volatile char&>(val)
			)
		);
	}
}