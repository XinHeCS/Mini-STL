#pragma once

// Internal use
// Helper type traits of 
// standard type traits

#include <Config/Config.h>
/**
 *	Since some type traits technologies
 *	require compiling support. We need
 *	to borrow some skills from Cpp STL.
*/
#include <type_traits>

namespace MSTD {

	// Interal use
	// whether T is a integral type
	template<typename T>
	struct _isIntegral
	{
		static constexpr bool value = false;
	};

	template<>
	struct _isIntegral<bool>
	{
		static constexpr bool value = true;
	};	

	template<>
	struct _isIntegral<char>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<unsigned char>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<signed char>
	{
		static constexpr bool value = true;	
	};

	template<>
	struct _isIntegral<wchar_t>
	{
		static constexpr bool value = true;
	};
	
	template<>
	struct _isIntegral<char16_t>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<char32_t>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<short>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<unsigned short>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<unsigned int>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<int>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<unsigned long>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<long>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isIntegral<unsigned long long>
	{
		static constexpr bool value = true;
	};

	// Internal use
	// whether T is floating type
	template<typename T>
	struct _isFloating
	{
		static constexpr bool value = false;
	};

	template<>
	struct _isFloating<float>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isFloating<double>
	{
		static constexpr bool value = true;
	};

	template<>
	struct _isFloating<long double>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	struct _isEnum : std::is_enum<T> {};

	template<typename T>
	struct _isUnion : std::is_union<T> {};

	template<typename T>
	struct _isClass
	{
		template<typename T>
		static char test(int T::*);

		template<typename T>
		static long test(...);

		static constexpr bool value =
			(sizeof(test<T>(nullptr)) == 1) && !isUnion<T>::value;
	};

	// Whether a type is a function
	template<typename>
	struct _isFunction
	{
		static constexpr bool value = false;
	};

	// specialization for regular functions
	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...)>
	{
		static constexpr bool value = true;
	};

	// specialization for variadic functions such as std::printf
	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...)>
	{
		static constexpr bool value = true;
	};

	// specialization for function types that have cv-qualifiers
	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const> 
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile>
	{
		static constexpr bool value = true;
	};


	// specialization for function types that have ref-qualifiers
	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile &>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) && >
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const &&>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile &&>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile &&>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) && >
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const &&>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile &&>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile &&>
	{
		static constexpr bool value = true;
	};


	// specializations for noexcept versions of all the above (C++17 and later)
#if CPP_STD >= 17
	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile & noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) volatile && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args...) const volatile && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) volatile && noexcept>
	{
		static constexpr bool value = true;
	};

	template<class Ret, class... Args>
	struct _isFunction<Ret(Args..., ...) const volatile && noexcept>
	{
		static constexpr bool value = true;
	};
#endif

	template<typename T>
	struct _isTrivial : std::is_trivial<T> {};

	template<typename T>
	struct _isTriviallyCopyable : std::is_trivially_copyable<T> {};

	template<typename T>
	struct _isStandardLayout : std::is_standard_layout<T> {};

	template<typename T>
	struct _isPod : std::is_pod<T> {};

	template<typename T>
	struct _isEmpty : std::is_empty<T> {};

	template<typename T>
	struct _isPolymorphic : std::is_polymorphic<T> {};

	template<typename T>
	struct _isAbstract : std::is_abstract<T> {};

#if CPP_STD >= 14
	template<typename T>
	struct _isFinal : std::is_final<T> {};
#endif

#if CPP_STD >= 17
	template<typename T>
	struct _isAggregate : std::is_aggregate<T> {};
#endif

	// utility to judge whether can be construct by Args... params or not
	template<typename... Args>
	struct _makeVoid
	{
		using type = void;
	};

	template<typename... Args>
	using _voidType = typename _makeVoid<Args...>::type;

	template<typename T, typename, typename... Args>
	struct _construct
	{
		static constexpr bool value = false;
	};

	template<typename T, typename... Args>
	struct _construct<
		T,
		_voidType<decltype(new T(std::declval<Args>()...))>,
		Args...
	>
	{
		static constexpr bool value = true;
	};

	// Constructor
	template<typename T, typename... Args>
	struct __construct : std::is_constructible<T, Args...> {};

	template<typename T, typename... Args>
	struct __triviallyConstruct : std::is_trivially_constructible<T, Args...> {};

	template<typename T, typename... Args>
	struct __nothrowConstruct : std::is_nothrow_constructible<T, Args...> {};

	// Default constructor
	template<typename T>
	struct __defaultConstruct : std::is_default_constructible<T> {};

	template<typename T>
	struct __triviallyDefaultConstruct : std::is_trivially_default_constructible<T> {};

	template<typename T, typename... Args>
	struct __nothrowDefaultConstruct : std::is_nothrow_default_constructible<T> {};

	// Copy constructor
	template<typename T>
	struct __copyConstruct : std::is_copy_constructible<T> {};

	template<typename T>
	struct __triviallyCopyConstruct : std::is_trivially_copy_constructible<T> {};

	template<typename T>
	struct __nothrowCopyConstruct : std::is_nothrow_copy_constructible<T> {};

	// Move constructor
	template<typename T>
	struct __moveConstruct : std::is_move_constructible<T> {};

	template<typename T>
	struct __triviallyMoveConstruct : std::is_trivially_move_constructible<T> {};

	template<typename T>
	struct __nothrowMoveConstruct : std::is_nothrow_move_constructible<T> {};

	// assignable
	template< class T, class U >
	struct __isAssignble : std::is_assignable<T, U> {};

	template< class T, class U >
	struct __isTrviallyAssignble : std::is_trivially_assignable<T, U> {};

	template< class T, class U >
	struct __isNothrowAssignble : std::is_nothrow_assignable<T, U> {};

	// copy assignable
	template< class T>
	struct __isCopyAssignble : std::is_copy_assignable<T> {};

	template< class T>
	struct __isTrviallyCopyAssignble : std::is_trivially_copy_assignable<T> {};

	template< class T>
	struct __isNothrowCopyAssignble : std::is_nothrow_copy_assignable<T> {};

	// move assignable
	template< class T>
	struct __isMoveAssignble : std::is_move_assignable<T> {};

	template< class T>
	struct __isTrviallyMoveAssignble : std::is_trivially_move_assignable<T> {};

	template< class T>
	struct __isNothrowMoveAssignble : std::is_nothrow_move_assignable<T> {};

	// Destructor
	template<typename T>
	struct __isDestructible : std::is_destructible<T> {};

	template<typename T>
	struct __isTriviallyDestructible : std::is_trivially_destructible<T> {};

	template<typename T, typename... Args>
	struct __isNothrowDestructible : std::is_nothrow_destructible<T> {};

	// has virtual destructor
	template<typename T>
	struct __hasVirtualDestructor : std::has_virtual_destructor<T> {};

	// Check convertible
	namespace detail {
		template<typename To>
		char test(To);

		template<typename To>
		long test(...);

		template<typename From, typename To>
		using _tryConvert =
			decltype(test<To>(std::declval<From>()));
	}

	template<typename From, typename To>
	struct _convertible
	{
		static constexpr bool value = 
			sizeof(detail::_tryConvert<From, To>) == sizeof(char);
	};

	template<>
	struct _convertible<void, void>
	{
		static constexpr bool value = true;
	};

	namespace baseDetails {
		template<typename To>
		char test(const To *);

		template<typename To>
		long test(...);

		template<typename From, typename To>
		using _tryConvert =
			decltype(test<To>(std::declval<From*>()));

		template<typename From, typename To, typename = void>
		struct _baseOf
		{
			static constexpr bool value = true;
		};

		template<typename From, typename To>
		struct _baseOf<From, To, _voidType<_tryConvert<From, To>>>
		{
			static constexpr bool value =
				sizeof(_tryConvert<From, To>) == sizeof(char);
		};
	}

	template<typename T>
	struct _makeSigned : std::make_signed<T> {};

	template<typename T>
	struct _makeUnsigned : std::make_unsigned<T> {};

	template<typename T>
	struct _decay 
	{
		using type = T;
	};

	template<typename T>
	struct _decay<T[]>
	{
		using type = T*;
	};

	template<typename T, size_t N>
	struct _decay<T[N]>
	{
		using type = T*;
	};

	template<typename Ret, typename... Args>
	struct _decay<Ret(Args...)>
	{
		using type = Ret (*)(Args...);
	};
}