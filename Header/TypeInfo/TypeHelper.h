#pragma once

// Internal use
// Some useful utilities for type trait

#include <TypeInfo/TypeTraits.h>

namespace MSTD {
	template<typename T>
	typename addRvalueReference<T>::type declval() noexcept;
}

// template to judge whether type {T}
// has member type {TypeName}:
// if true : define member {exist} to true;
// Otherwise : define member {exist} to false;
#define REGISTER_TYPE_CHECK(TypeName)				\
		template<typename T>						\
		struct _##TypeName##_Type_					\
		{											\
		private:									\
			template<typename T>					\
			static auto check(T)					\
			-> decltype(MSTD::declval<typename T::TypeName>(), \
						MSTD::trueType{});			\
													\
			template<typename T>					\
			static MSTD::falseType					\
				check(...);							\
		public:										\
			static constexpr bool exist =			\
				decltype(check<T>(MSTD::declval<T>()))::value;		\
		};		

// template to judge whether type {T}
// has member type {TypeName}:
// if true : define member {type} to TypeName;
// Otherwise : define member {type} to void;
#define REGISTER_TYPE_TRAIT(TypeName)				\
		template<typename T>						\
		struct _##TypeName##_Trait_					\
		{											\
		private:									\
			template<typename T>					\
			static typename T::TypeName				\
				check(T);							\
													\
			template<typename T>					\
			static void								\
				check(...);							\
		public:										\
			using type =							\
				decltype(check<T>(MSTD::declval<T>()));	\
		};


// Check whether class T has a member function named {funcName}
// with signature {Sign}
// if true : define member {exist} to true;
// Otherwise : define member {exist} to false;
#define REGISTER_FUNCTION_CHECK(funcName)		\
		template<typename T, typename Ret, typename... Args>									\
		struct _has_##funcName																	\
		{																						\
		private:																				\
			template<typename T>																\
			static auto check(T*)																\
				-> decltype(MSTD::declval<T>().funcName(MSTD::declval<Args>()...));				\
																								\
			template<typename T>																\
			static auto check(...)																\
				-> decltype(MSTD::declval<MSTD::falseType>());									\
		public:																					\
			static constexpr bool exist =														\
				MSTD::isSame<decltype(check<T>(nullptr)), Ret>::value;							\
		};																						\
		template<typename T, typename Sign>														\
		struct _##funcName##_Func_ { static constexpr bool exist = false; };					\
		template<typename T, typename Ret, typename... Args>									\
		struct _##funcName##_Func_<T, Ret(T::*)(Args...)> :_has_##funcName##<T, Ret, Args...> {};	\
		template<typename T, typename Ret, typename... Args>									\
		struct _##funcName##_Func_<T, Ret(T::*)(Args...) const> :_has_##funcName##<T, Ret, Args...> {};
