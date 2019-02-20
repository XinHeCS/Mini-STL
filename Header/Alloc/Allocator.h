#pragma once

// Allocator standard header

#include <Config/Config.h>
#include <Iterator/Iterator.h>
#include <TypeInfo/TypeTraits.h>
#include <TypeInfo/TypeHelper.h>
#include <Utility/forward.h>
#include <Utility/addressof.h>
#include <Alloc/MiniAlloc.h>

namespace MSTD {
#ifdef USE_DIRECT_MALLOC
	using alloc = baseAlloc;
#else
	using alloc = defaultAlloc;
#endif // USE_DIRECT_MALLOC

	template<typename T>
	class Allocator {
		static_assert(!isConst<T>::value,
			"Const type is ill formed to be allocated");

	public:
		using ValueType = T;
		using Pointer = T * ;
		using ConstPointer = const T*;
		using Reference = T & ;
		using ConstReference = const T&;
		using SizeType = size_t;
		using DifferenceType = ptrdiff_t;
		/*using reBind = Allocator<U>;*/

		// Constructor
		Allocator() = default;
		Allocator(const Allocator &) = default;

		Pointer address(Reference lvalue) const noexcept { return MSTD::addressof(lvalue); }
		ConstPointer address(ConstReference lvalue) const noexcept { return MSTD::addressof(lvalue); }

		T* allocate(SizeType n)
		{
			return static_cast<T*>(alloc::alloc(n * sizeof(T)));
		}
			
		void deallocate(Pointer p, SizeType n)
		{
			alloc::dealloc(p, n * sizeof(T));
		}

		constexpr SizeType maxSize() const noexcept
		{
			return static_cast<SizeType>(-1) / sizeof(T);
		}

		template<typename U, typename... Args>
		void construct(U *p, Args&&... args)
		{
			::new ((void*)p) U(MSTD::forward<Args>(args)...);
		}

		template<typename U>
		void destroy(U *p)
		{
			p->~U();
		}
	};

	template<typename T>
	inline constexpr
	bool operator==(const Allocator<T>&, const Allocator<T>&) noexcept { return true; }	
	
	template<typename T>
	inline constexpr
	bool operator!=(const Allocator<T>&, const Allocator<T>&) noexcept { return false; }

	// allocator traits

	template<typename Alloc>
	class AllocatorTraits
	{
		REGISTER_TYPE_TRAIT(Pointer);
		REGISTER_TYPE_TRAIT(ConstPointer);
		REGISTER_TYPE_TRAIT(VoidPoiner);
		REGISTER_TYPE_TRAIT(ConstVoidPointer);
		REGISTER_TYPE_TRAIT(DifferenceType);
		REGISTER_TYPE_TRAIT(SizeType);

		REGISTER_FUNCTION_CHECK(construct);
		REGISTER_FUNCTION_CHECK(destroy);
	public:
		using AllocatorType = Alloc;
		using ValueType = typename Alloc::ValueType;
		using Pointer = typename _Pointer_Trait_<Alloc>::type;
		using ConstPointer = typename _ConstPointer_Trait_<Alloc>::type;
		using VoidPointer = typename _VoidPoiner_Trait_<Alloc>::type;
		using ConstVoidPointer = typename _ConstVoidPointer_Trait_<Alloc>::type;
		using DifferenceType = typename _DifferenceType_Trait_<Alloc>::type;
		using SizeType = typename _SizeType_Trait_<Alloc>::type;

		template<typename Other>
		using rebind = Allocator<Other>;

		template<typename Other>
		using rebindTraits = AllocatorTraits<Allocator<Other>>;

		static Pointer allocate(Alloc &a, SizeType n)
		{
			return static_cast<Pointer>(a.allocate(n));
		}

		static void deallocate(Alloc& a, Pointer p, SizeType n = 1)
		{
			a.deallocate(p, n);
		}

		// {a} has member construct
		template<typename T, typename... Args>
		static void _auxConstruct(trueType, Alloc &a, T *p, Args&&... args)
		{
			a.construct(p, MSTD::forward<Args>(args)...);
		}

		// {a} doesn't have member construct
		template<typename T, typename... Args>
		static void _auxConstruct(falseType, Alloc &a, T *p, Args&&... args)
		{
			::new (static_cast<void*>(p)) T(MSTD::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		static void construct(Alloc &a, T *p, Args&&... args)
		{
			_auxConstruct(
				typename conditional<_construct_Func_<Alloc, void(Alloc::*)(Args...)>::exist, 
									trueType, falseType>::type(),
				a, p, args...);
		}

		// {a} has member destroy
		template<typename T>
		static void _auxDestroy(trueType, Alloc &a, T *p)
		{
			a.destroy(p);
		}

		// {a} doesn't have member destroy
		template<typename T>
		static void _auxDestroy(falseType, Alloc &a, T *p)
		{
			p->~T();
		}

		template<typename T>
		static void destroy(Alloc &a, T *p)
		{
			_auxDestroy(
				typename conditional<_destroy_Func_<Alloc, void(Alloc::*)(T*)>::exist,
									trueType, falseType>::type(),
				a, p);
		}

		static SizeType maxSize(Alloc &a) { return static_cast<SizeType>(-1) / sizeof(ValueType); }
	};


	template<typename InputIter, typename ForwardIter>
	ForwardIter _aux_UninitializedCopy(InputIter first, InputIter last,
										ForwardIter dest, falseType)
	{	// Can't be trivially constructed
		using ValueType = typename IteratorTraits<ForwardIter>::ValueType;
		ForwardIter current = dest;
		_MSTD_TRY
			for (auto it = first; it != last; ++it, ++current) {
				::new (static_cast<void*>(MSTD::addressof(*current))) ValueType(*it);
			}
		_MSTD_CATCH_ALL
			for (auto it = dest; it != current; ++it) {
				it->~ValueType();
			}
			throw;
		_MSTD_END_CATCH
		return current;
	}

	//template<typename InputIter, typename ForwardIter>
	//ForwardIter _aux_UninitializedCopy(InputIter first, InputIter last,
	//									ForwardIter dest, trueType)
	//{	// Can be trivially constructed
	//	using ValueType = typename IteratorTraits<ForwardIter>::ValueType;
	//	auto count = last - first;		
	//	std::memmove(static_cast<void*>(MSTD::addressof(*dest)),
	//				 static_cast<void*>(MSTD::addressof(*first)),
	//				 count * sizeof(ValueType));
	//	return dest + count;
	//}

	/**
	 *	Copies elements from the range [first, last) to an 
	 *	uninitialized memory area beginning at {d_first}
	 *
	 *	@param first -	the range of the elements to copy
	 *	@param last -	the range of the elements to copy
	 *	@param dest	- the beginning of the destination range
	 *	
	 *	@return
	 *	Iterator to the element past the last element copied.
	*/
	template<typename InputIter, typename ForwardIter>
	ForwardIter uninitializedCopy(InputIter first, InputIter last, ForwardIter dest)
	{
		using ValueType = typename IteratorTraits<ForwardIter>::ValueType;
		using ValueTypeSrc = typename IteratorTraits<InputIter>::ValueType;

		static_assert(isConvertible<ValueTypeSrc, ValueType>::value,
					  "Uncompatible value type(SrcType -> DestType");

		using InputCat = typename IteratorTraits<InputIter>::IteratorCategory;
		using ForwardCat = typename IteratorTraits<ForwardIter>::IteratorCategory;
		return _aux_UninitializedCopy(
			first, last, dest,
			typename conditional<
				isSame<ValueType, ValueTypeSrc>::value				&&
				isSame<RandomAccessIteratorTag, InputCat>::value	&&
				isSame<RandomAccessIteratorTag, ForwardCat>::value	&&
				isTriviallyCopyConstructible<ValueType>::value,
				/*trueType*/falseType, falseType
			>::type()
		);
	}


	template<typename InputIter, typename Size, typename ForwardIter>
	ForwardIter _aux_UninitializedCopy_N(InputIter first, Size n,
										 ForwardIter dest, InputIteratorTag)
	{
		auto last = first;
		advance(last, n);
		return uninitializedCopy(first, last, dest);
	}

	template<typename InputIter, typename Size, typename ForwardIter>
	ForwardIter _aux_UninitializedCopy_N(InputIter first, Size n,
										 ForwardIter dest, RandomAccessIteratorTag)
	{
		return uninitializedCopy(first, first + n, dest);
	}

	/**
	 *	Copies elements from the range [first, first + n) to an
	 *	uninitialized memory area beginning at {d_first}
	 *
	 *	@param first - the range of the elements to copy	 
	 *	@param n - the number of the elements to copy
	 *	@param dest	- the beginning of the destination range
	 *	
	 *	@return
	 *	Iterator to the element past the last element copied.
	*/
	template<typename InputIter, typename Size, typename ForwardIter>
	ForwardIter uninitializedCopy_N(InputIter first, Size n, ForwardIter dest)
	{
		if (n > 0) {
			dest = _aux_UninitializedCopy_N(
				first, n, dest,
				typename IteratorTraits<InputIter>::IteratorCategory()
			);
		}

		return dest;
	}

	/**
	 *	Copies the given value to an uninitialized memory area, 
	 *	defined by the range [first, last) 
	 *
	 *	@param first -	the range of the elements to fill
	 *	@param last -	the range of the elements to fill
	 *	@param val -	the value to fill the elements with
	*/
	template<typename InputIter, typename T>
	void uninitializedFill(InputIter first, InputIter last, const T &val)
	{	// Can't be trivially constructed
		using ValueType = typename IteratorTraits<InputIter>::ValueType;
		auto beg = first;
		_MSTD_TRY
			for (; first != last; ++first) {
				::new (static_cast<void*>(MSTD::addressof(*first))) ValueType(val);
			}
		_MSTD_CATCH_ALL
			for (auto it = beg; it != first; ++it) {
				it->~ValueType();
			}
			throw;
		_MSTD_END_CATCH
	}

	/**
	 *	Copies the given value to an uninitialized memory area,
	 *	defined by the range [first, first + n)
	 *
	 *	@param first -	the range of the elements to fill
	 *	@param n	-	the nuumber of the elements to fill
	 *	@param val	-	the value to construct the elements with
	*/
	template<typename InputIter, typename Size, typename T>
	void uninitializedFill_N(InputIter first, Size n, const T &val)
	{	// Can't be trivially constructed
		using ValueType = typename IteratorTraits<InputIter>::ValueType;
		auto beg = first;
		_MSTD_TRY
			for (Size count = 1; count <= n; ++first, ++count) {
				::new (static_cast<void*>(MSTD::addressof(*first))) ValueType(val);
			}
		_MSTD_CATCH_ALL
			for (auto it = beg; it != first; ++it) {
				it->~ValueType();
			}
			throw;
		_MSTD_END_CATCH
	}
}