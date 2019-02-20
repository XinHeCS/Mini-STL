#pragma once

// STL move tool

#include <TypeInfo/TypeTraits.h>

namespace MSTD {

	template<typename T>
	typename removeReference<T>::type&&
		move(T&& val)
	{
		return static_cast<typename removeReference<T>::type&&>(val);
	}

}