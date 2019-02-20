#pragma once

// type_traits standard header

#include <Config/Config.h>
#include <TypeInfo/TypeDetail.h>

namespace MSTD {

	// =============== Helper Classes ===============

	template<typename T,
		T _Val>
		struct integralConstant
	{
		static constexpr T value = _Val;

		using ValueType = T;
		using type = integralConstant;

		constexpr operator ValueType() const noexcept
		{
			return value;
		}

#if CPP_STD >= 14
		constexpr valueType operator()() const noexcept
		{
			return value;
		}
#endif
	};

#if CPP_STD >= 17
	template<bool B>
	using boolConstant = integralConstant<bool, B>;
#endif

	// Some useful integral constant
	using trueType = integralConstant<bool, true>;
	using falseType = integralConstant<bool, false>;

	// =============== Const - volatility specifiers ===============

	template<typename T>
	struct removeConst
	{
		using type = T;
	};

	template<typename T>
	struct removeConst<const T>
	{
		using type = T;
	};

	template<typename T>
	struct removeVolatile
	{
		using type = T;
	};

	template<typename T>
	struct removeVolatile<volatile T>
	{
		using type = T;
	};

	template<typename T>
	struct removeCV 
		: removeConst<typename removeVolatile<T>::type> {};

	template<typename T>
	struct addConst
	{
		using type = const T;
	};

	template<typename T>
	struct addVolatile
	{
		using type = volatile T;
	};

	template<typename T>
	struct addCV : addConst<typename addVolatile<T>::type> {};

	// =============== Type relationships ===============

	// Judge whether two types are the same
	// types(with _cv qualifier) or not
	template<typename T,
		typename U>
		struct isSame : falseType {};

	template<typename T>
	struct isSame<T, T> : trueType {};

	template<typename From, typename To>
	struct isConvertible 
		: _convertible<typename removeCV<From>::type, 
						typename removeCV<To>::type> {};


	// =============== References ===============

	template<typename T>
	struct removeReference
	{
		using type = T;
	};

	template<typename T>
	struct removeReference<T&>
	{
		using type = T;
	};

	template<typename T>
	struct removeReference<T&&>
	{
		using type = T;
	};

	template<typename T>
	struct addLvalueReference
	{
		using type = T&;
	};

	template<>
	struct addLvalueReference<void>
	{
		using type = void;
	};

	template<>
	struct addLvalueReference<const void>
	{
		using type = const void;
	};

	template<>
	struct addLvalueReference<volatile void>
	{
		using type = volatile void;
	};

	template<>
	struct addLvalueReference<const volatile void>
	{
		using type = const volatile void;
	};

	template<typename T>
	struct addRvalueReference
	{
		using type = T &&;
	};

	template<>
	struct addRvalueReference<void>
	{
		using type = void;
	};

	template<>
	struct addRvalueReference<const void>
	{
		using type = const void;
	};

	template<>
	struct addRvalueReference<volatile void>
	{
		using type = volatile void;
	};

	template<>
	struct addRvalueReference<const volatile void>
	{
		using type = const volatile void;
	};

	// =============== Pointers ===============

	template< class T > struct removePointer { typedef T type; };
	template< class T > struct removePointer<T*> { typedef T type; };
	template< class T > struct removePointer<T* const> { typedef T type; };
	template< class T > struct removePointer<T* volatile> { typedef T type; };
	template< class T > struct removePointer<T* const volatile> { typedef T type; };

	template<typename T>
	struct addPointer
	{
		using type = T*;
	};

	template<typename T>
	struct addPointer<T&>
	{
		using type = T*;
	};

	template<typename T>
	struct addPointer<T&&>
	{
		using type = T*;
	};

	// =============== Primary type categories ===============

	// Whether T is a void type(with qualifiers)
	// Note: void* is not a void type
	template<typename T>
	struct isVoid : isSame<void, typename removeCV<T>::type> {};

#if CPP_STD >= 14
	template<typename T>
	struct isNullPointer 
		: isSame<std::nullptr_t, typename removeCV<T>::type> {};
#endif

	template<typename T>
	struct isIntegral : _isIntegral<typename removeCV<T>::type> {};

	template<typename T>
	struct isFloating : _isFloating<typename removeCV<T>::type> {};

	template<typename T>
	struct isArray : falseType {};

	template<typename T>
	struct isArray<T[]> : trueType {};

	template<typename T, std::size_t N>
	struct isArray<T[N]> : trueType {};

	template<typename T>
	struct isEnum : _isEnum<T> {};

	template<typename T>
	struct isUnion : _isUnion<T> {};

	template<typename T>
	struct isClass : _isClass<T> {};

	template<typename Base, typename Derived>
	struct isBasef : integralConstant<bool,
		isClass<Base>::value		&&
		isClass<Derived>::value		&&
		baseDetails::_baseOf<typename removeCV<Derived>::type,
							 typename removeCV<Base>::type>::value
	> {};

	template<typename T>
	struct isFunction : _isFunction<T> {};

	template<typename T>
	struct isPointer : falseType {};

	template<typename T>
	struct isPointer<T *> : trueType {};

	template<typename T>
	struct isLvalueReference : falseType {};

	template<typename T>
	struct isLvalueReference<T&> : trueType {};

	template<typename T>
	struct isRvalueReference : falseType {};

	template<typename T>
	struct isRvalueReference<T&&> : trueType {};

	template<typename T>
	struct isMemberPointer : falseType {};

	template<typename T, typename U>
	struct isMemberPointer<U T::*> : trueType {};

	template<typename T>
	struct isMemberDataPointer : falseType {};

	template<typename T, typename U>
	struct isMemberDataPointer<U T::*> : integralConstant<bool,
											!isFunction<U>::value> {};

	template<typename T>
	struct isMemberFunctionPointer : falseType {};

	template<typename T, typename U>
	struct isMemberFunctionPointer<U T::*> : integralConstant<bool,
												isFunction<U>::value> {};
	
	// =============== Composite type categories ===============

	template<typename T>
	struct isArithmetic : integralConstant<bool, 
							isIntegral<typename removeCV<T>::type>::value ||
							isFloating<typename removeCV<T>::type>::value> {};

	template<typename T>
	struct isFundamental : integralConstant<bool,
							isArithmetic<T>::value	||
							isVoid<T>::value		||
#if CPP_STD <= 11
							isSame<std::nullptr_t, typename removeCV<T>::type>::value> {};
#else
							isNullPointer<T>::value> {};
#endif

	template<typename T>
	struct isScalar : integralConstant<bool, 
						isArithmetic<T>::value		||
						isPointer<T>::value			||
						isMemberPointer<T>::value	||
						isEnum<T>::value			||
#if CPP_STD <= 11
						isSame<std::nullptr_t, 
							typename removeCV<T>::type>::value> {};
#else
						isNullPointer<T>::value> {};
#endif

	template<typename T>
	struct isCompound : integralConstant<bool, 
							!isFundamental<T>::value> {};

	template< class T>
	struct isObject : integralConstant<bool,
						isScalar<T>::value	||
						isArray<T>::value	||
						isUnion<T>::value	||
						isClass<T>::value> {};

	template<typename T>
	struct isReference : falseType {};

	template<typename T>
	struct isReference<T&> : trueType {};

	template<typename T>
	struct isReference<T&&> : trueType {};

	// =============== Type properties ===============

	template<typename T>
	struct isConst : falseType {};

	template<typename T>
	struct isConst<const T> : trueType {};

	template<typename T>
	struct isVolatile : falseType {};

	template<typename T>
	struct isVolatile<volatile T> : trueType {};

	template<typename T>
	struct isTrivial : _isTrivial<T> {};

	template<typename T>
	struct isTriviallyCopyable : _isTriviallyCopyable<T> {};

	template<typename T>
	struct isStandardLayout : _isStandardLayout<T> {};

	template<typename T>
	struct isPod : _isPod<T> {};

	template<typename T>
	struct isEmpty : _isEmpty<T> {};

	template<typename T>
	struct isPolymorphic : _isPolymorphic<T> {};

	template<typename T>
	struct isAbstract : _isAbstract<T> {};

#if CPP_STD >= 14
	template<typename T>
	struct isFinal : _isFinal<T> {};
#endif

#if CPP_STD >= 17
	template<typename T>
	struct isAggregate : _isAggregate<T> {};
#endif

	template<typename T>
	struct isSigned : integralConstant<bool, 
						isArithmetic<T>::value &&
						(T(-1) < T(0))> {};

	template<typename T>
	struct isUnsigned : integralConstant<bool,
						isArithmetic<T>::value &&
						(T(0) < T(-1))> {};

	// =============== Supported operations ===============

	template<typename T, typename... Args>
	struct isConstructible : __construct<T, Args...> {};

	template<typename T, typename... Args>
	struct isTriviallyConstructible : __triviallyConstruct<T, Args...> {};

	template<typename T, typename... Args>
	struct isNothrowConstructible : __nothrowConstruct<T, Args...> {};

	template<typename T>
	struct isDefaultConstructible : __defaultConstruct<T> {};

	template<typename T>
	struct isTriviallyDefaultConstructible : __triviallyDefaultConstruct<T> {};

	template<typename T, typename... Args>
	struct isNothrowDefaultConstructible : __nothrowDefaultConstruct<T> {};

	template<typename T>
	struct isCcopyConstructible : __copyConstruct<T> {};

	template<typename T>
	struct isTriviallyCopyConstructible : __triviallyCopyConstruct<T> {};

	template<typename T>
	struct isNothrowCopyConstructible : __nothrowCopyConstruct<T> {};

	template<typename T>
	struct isMoveConstructible : __moveConstruct<T> {};

	template<typename T>
	struct isTriviallyMoveConstructible : __triviallyMoveConstruct<T> {};

	template<typename T>
	struct isNothrowMoveConstructible : __nothrowMoveConstruct<T> {};

	template< class T, class U >
	struct isAssignble : __isAssignble<T, U> {};

	template< class T, class U >
	struct isTrviallyAssignable : __isTrviallyAssignble<T, U> {};

	template< class T, class U >
	struct isNothrowAssignble : __isNothrowAssignble<T, U> {};

	template< class T>
	struct isCopyAssignble : __isCopyAssignble<T> {};

	template< class T>
	struct isTrviallyCopyAssignble : __isTrviallyCopyAssignble<T> {};

	template< class T>
	struct isNothrowCopyAssignble : __isNothrowCopyAssignble<T> {};

	template< class T>
	struct isMoveAssignble : __isMoveAssignble<T> {};

	template< class T>
	struct isTrviallyMoveAssignble : __isTrviallyMoveAssignble<T> {};

	template< class T>
	struct isNothrowMoveAssignble : __isNothrowMoveAssignble<T> {};

	template<typename T>
	struct isDestructible : __isDestructible<T> {};

	template<typename T>
	struct isTriviallyDestructible : __isTriviallyDestructible<T> {};

	template<typename T, typename... Args>
	struct isNothrowDestructible : __isNothrowDestructible<T> {};

	template<typename T>
	struct hasVirtualDestructor : __hasVirtualDestructor<T> {};

	// =============== Property queries ===============

	template<typename T>
	struct alignmentOf : integralConstant<size_t, alignof(T)> {};

	template<typename T>
	struct rank : integralConstant<size_t, 0> {};

	template<typename T>
	struct rank<T[]> : integralConstant<size_t, rank<T>::value + 1> {};

	template<typename T, size_t N>
	struct rank<T[N]> : integralConstant<size_t, rank<T>::value + 1> {};

	template<typename T, size_t N>
	struct extent : integralConstant<size_t, 0> {};

	template<typename T, size_t D>
	struct extent<T[D], 0> : integralConstant<size_t, D> {};

	template<typename T>
	struct extent<T[], 0> : integralConstant<size_t, 0> {};

	template<typename T, size_t N, size_t D>
	struct extent<T[D], N> : extent<T, N - 1> {};

	template<typename T, size_t N>
	struct extent<T[], N> : extent<T, N - 1> {};

	// =============== Sign modifiers ===============

	template<typename T>
	struct makeSigned : _makeSigned <T> {};

	template<typename T>
	struct makeUnsigned : _makeUnsigned<T> {};

	// =============== Arrays ===============

	template<typename T>
	struct removeExtent
	{
		using type = T;
	};

	template<typename T>
	struct removeExtent<T[]>
	{
		using type = T;
	};

	template<typename T, size_t N>
	struct removeExtent<T[N]>
	{
		using type = T;
	};

	template<typename T>
	struct removeAllExtents
	{
		using type = T;
	};

	template<typename T>
	struct removeAllExtents<T[]> : removeAllExtents<T> {};

	template<typename T, size_t N>
	struct removeAllExtents<T[N]> : removeAllExtents<T> {};

	// =============== Miscellaneous transformations ===============

	template<typename T>
	struct decay : _decay<typename removeCV<typename removeReference<T>::type>::type> {};

	template<bool B, typename T = void>
	struct enableIf {};

	template<typename T>
	struct enableIf<true, T>
	{
		using type = T;
	};

	template<bool B, typename T, typename F>
	struct conditional
	{
		using type = F;
	};

	template<typename T, typename F>
	struct conditional<true, T, F>
	{
		using type = T;
	};

	// =============== Operations on traits ===============

	// TODO...
}