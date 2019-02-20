#pragma once

// STL forward tool

#include <TypeInfo/TypeTraits.h>

namespace MSTD {
	template<typename T>
	constexpr T&& forward(typename removeReference<T>::type& val)
	{
		return static_cast<T&&>(val);
	}

	template<typename T>
	constexpr T&& forward(typename removeReference<T>::type&& val)
	{
		return static_cast<T&&>(val);
	}
}