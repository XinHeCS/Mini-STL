#pragma once

// Set standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>
#include <Container/Internal/_RBTree.h>

// STL header and cpp standard header
#include <initializer_list>
#include <stdexcept>

namespace MSTD {

	template<		
		typename _Val,
		typename _Compare,
		typename _Alloc,
		bool _M // Enable multi keys insertion
	> struct _SetConfig
	{
		using DifferenceType = ptrdiff_t;
		using ValueType = _Val;
		using ConstReference = const ValueType&;
		using ConstPointer = const ValueType*;
		using KeyType = _Val;
		using Reference = _Val & ;
		using Pointer = ValueType * ;
		using SizeType = size_t;
		using KeyCompare = _Compare;
		using AllocatorType = _Alloc;

		enum
		{
			MULTI = _M
		};

		static const KeyType& getKeyFromVal(const ValueType &val)
		{
			return val;
		}
	};

	template<
		typename _Key,
		typename _Compare = std::less<_Key>,
		typename Alloc = MSTD::Allocator<_Key>
	> class Set
		: public _RBTree<_SetConfig<_Key, _Compare, Alloc, false>>
	{
		using _Config = _SetConfig<_Key, _Compare, Alloc, false>;
		using _Base = _RBTree<_Config>;
	public:
		using DifferenceType = typename _Config::DifferenceType;
		using ConstReference = typename _Config::ConstReference;
		using ConstPointer = typename _Config::ConstPointer;
		using ValueType = typename _Config::ValueType;
		using KeyType = typename _Config::KeyType;
		using Reference = typename _Config::Reference;
		using Pointer = typename _Config::Pointer;
		using SizeType = typename _Config::SizeType;
		using KeyCompare = typename _Config::KeyCompare;
		using AllocatorType = typename _Config::AllocatorType;

		static_assert(
			MSTD::isSame<ValueType, typename AllocatorType::ValueType>::value,
			"Map<Key, Val, Comp, Alloc> isn't capable with Allocator"
			);

		using ConstIterator = typename _Base::ConstIterator;
		using Iterator = typename _Base::Iterator;
		using ConstReverseIterator = typename _Base::ConstReverseIterator;
		using ReverseIterator = typename _Base::ReverseIterator;

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		////////////////////////////////////
		Set() :
			_Base()
		{}

		explicit Set(const KeyCompare &comp,
			const AllocatorType &alloc = AllocatorType()) :
			_Base(comp, alloc)
		{}

		template<typename InputIt>
		Set(InputIt first, InputIt last,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(first, last, comp, alloc)
		{}

		Set(const Set &that) :
			_Base(that)
		{}

		Set(Set &&that) noexcept :
			_Base(MSTD::move(that))
		{}

		Set(std::initializer_list<ValueType> il,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(il, comp, alloc)
		{}

		Set& operator=(const Set &that)
		{
			_Base::operator=(that);
			return *this;
		}

		Set& operator=(Set &&that) noexcept
		{
			_Base::operator=(MSTD::move(that));
			return *this;
		}

		Set& operator=(std::initializer_list<ValueType> il)
		{
			_Base::operator=(il);
			return *this;
		}
	};

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator==(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		if (lhs.size() != rhs.size()) {
			return false;
		}
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (*lit++ != *rit++) {
				return false;
			}
		}
		return true;
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator!=(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator<(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (*lit++ >= *rit++) {
				return false;
			}
		}
		if (lit != lhs.end() || rit != rhs.end()) {
			return lhs.size() < rhs.size();
		}

		return true;
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator<=(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (*lit++ > *rit++) {
				return false;
			}
		}

		if (lit != lhs.end() || rit != rhs.end()) {
			return lhs.size() < rhs.size();
		}

		return true;
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator>(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator>=(const Set<_Key, _Compare, _Alloc> &lhs,
					const Set<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	void swap(Set<_Key, _Compare, _Alloc> &lhs,
			Set<_Key, _Compare, _Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<
		typename _Key,
		typename _Compare = std::less<_Key>,
		typename Alloc = MSTD::Allocator<_Key>
	> class MultiSet
		: public _RBTree<_SetConfig<_Key, _Compare, Alloc, true>>
	{
		using _Config = _SetConfig<_Key, _Compare, Alloc, true>;
		using _Base = _RBTree<_Config>;
	public:
		using DifferenceType = typename _Config::DifferenceType;
		using ConstReference = typename _Config::ConstReference;
		using ConstPointer = typename _Config::ConstPointer;
		using ValueType = typename _Config::ValueType;
		using KeyType = typename _Config::KeyType;
		using Reference = typename _Config::Reference;
		using Pointer = typename _Config::Pointer;
		using SizeType = typename _Config::SizeType;
		using KeyCompare = typename _Config::KeyCompare;
		using AllocatorType = typename _Config::AllocatorType;

		static_assert(
			MSTD::isSame<ValueType, typename AllocatorType::ValueType>::value,
			"Map<Key, Val, Comp, Alloc> isn't capable with Allocator"
			);

		using ConstIterator = typename _Base::ConstIterator;
		using Iterator = typename _Base::Iterator;
		using ConstReverseIterator = typename _Base::ConstReverseIterator;
		using ReverseIterator = typename _Base::ReverseIterator;

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		////////////////////////////////////
		MultiSet() :
			_Base()
		{}

		explicit MultiSet(const KeyCompare &comp,
			const AllocatorType &alloc = AllocatorType()) :
			_Base(comp, alloc)
		{}

		template<typename InputIt>
		MultiSet(InputIt first, InputIt last,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(first, last, comp, alloc)
		{}

		MultiSet(const MultiSet &that) :
			_Base(that)
		{}

		MultiSet(MultiSet &&that) noexcept :
			_Base(MSTD::move(that))
		{}

		MultiSet(std::initializer_list<ValueType> il,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(il, comp, alloc)
		{}

		MultiSet& operator=(const MultiSet &that)
		{
			_Base::operator=(that);
			return *this;
		}

		MultiSet& operator=(MultiSet &&that) noexcept
		{
			_Base::operator=(MSTD::move(that));
			return *this;
		}

		MultiSet& operator=(std::initializer_list<ValueType> il)
		{
			_Base::operator=(il);
			return *this;
		}
	};

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator==(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		if (lhs.size() != rhs.size()) {
			return false;
		}
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (!(*lit++ == *rit++)) {
				return false;
			}
		}
		return true;
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator!=(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator<(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (*lit < *rit) {
				return true;
			}
			else if (*rit++ < *lit++) {
				return false;
			}
		}

		return lit == lhs.end() && rit != rhs.end();
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator<=(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		auto lit = lhs.begin();
		auto rit = rhs.begin();
		while (lit != lhs.end() && rit != rhs.end()) {
			if (*lit < *rit) {
				return true;
			}
			else if (*rit++ < *lit++) {
				return false;
			}
		}

		return lit == lhs.end();
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator>(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	bool operator>=(const MultiSet<_Key, _Compare, _Alloc> &lhs,
					const MultiSet<_Key, _Compare, _Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename _Key, typename _Compare, typename _Alloc>
	void swap(MultiSet<_Key, _Compare, _Alloc> &lhs,
		MultiSet<_Key, _Compare, _Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}
}