#pragma once

// Map standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>
#include <Container/Internal/_RBTree.h>

// STL header and cpp standard header
#include <initializer_list>
#include <utility>
#include <stdexcept>

namespace MSTD {

	template<
		typename _Key,
		typename _Val,
		typename _Compare,
		typename _Alloc,
		bool _M // Enable multi keys insertion
	> struct _MapConfig
	{
		using DifferenceType = ptrdiff_t;
		using ValueType = std::pair<const _Key, _Val>;
		using ConstReference = const ValueType&;
		using ConstPointer = const ValueType*;
		using KeyType = _Key;
		using MappedType = _Val;
		using Reference = _Val&;
		using Pointer = ValueType*;
		using SizeType = size_t;
		using KeyCompare = _Compare;
		using AllocatorType = _Alloc;

		enum 
		{
			MULTI = _M
		};

		static const KeyType& getKeyFromVal(const ValueType &val)
		{
			return val.first;
		}
	};

	template<
		typename _Key,
		typename _Val,
		typename _Compare = std::less<_Key>,
		typename Alloc = MSTD::Allocator<std::pair<const _Key, _Val>>
	> class Map
		: public _RBTree<_MapConfig<_Key, _Val, _Compare, Alloc, false>>
	{
		using _Config = _MapConfig<_Key, _Val, _Compare, Alloc, false>;
		using _Base = _RBTree<_Config>;
	public:
		using DifferenceType = typename _Config::DifferenceType;
		using ConstReference = typename _Config::ConstReference;
		using ConstPointer = typename _Config::ConstPointer;
		using ValueType = typename _Config::ValueType;
		using KeyType = typename _Config::KeyType;
		using MappedType = typename _Config::MappedType;
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
		Map() :
			_Base()
		{}

		explicit Map(const KeyCompare &comp,
			const AllocatorType &alloc = AllocatorType()) :
			_Base(comp, alloc)
		{}

		template<typename InputIt>
		Map(InputIt first, InputIt last,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(first, last, comp, alloc)
		{}

		Map(const Map &that) :
			_Base(that)
		{}

		Map(Map &&that) noexcept :
			_Base(MSTD::move(that))
		{}

		Map(std::initializer_list<ValueType> il,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(il, comp, alloc)
		{}

		Map& operator=(const Map &that)
		{
			_Base::operator=(that);
			return *this;
		}

		Map& operator=(Map &&that) noexcept
		{
			_Base::operator=(MSTD::move(that));
			return *this;
		}

		Map& operator=(std::initializer_list<ValueType> il)
		{
			_Base::operator=(il);
			return *this;
		}

		/////////////////////////////////////
		//
		//			Element access
		//
		/////////////////////////////////////

		MappedType& at(const KeyType &key)
		{
			Iterator pos = _Base::find(key);
			if (pos != _Base::end()) {
				return (*pos).second;
			}
			else {
				throw std::out_of_range("No such key in map");
			}
		}

		const MappedType& at(const KeyType &key) const
		{
			Iterator pos = _Base::find(key);
			if (pos != _Base::end()) {
				return (*pos).second;
			}
			else {
				throw std::out_of_range("No such key in map");
			}
		}

		MappedType& operator[](const KeyType &key)
		{
			auto pos = _Base::find(key);
			if (pos != _Base::end()) {
				return pos->second;
			}
			else {
				auto ret = _Base::insert(ValueType(key, MappedType()));
				return ret.first->second;
			}
		}
	};

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator==(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator!=(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator<(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator<=(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator>(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator>=(const Map<_Key, _Val, _Compare, _Alloc> &lhs,
					const Map<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	void swap(Map<_Key, _Val, _Compare, _Alloc> &lhs,
				Map<_Key, _Val, _Compare, _Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}

	template<
		typename _Key,
		typename _Val,
		typename _Compare = std::less<_Key>,
		typename Alloc = MSTD::Allocator<std::pair<const _Key, _Val>>
	> class MultiMap
		: public _RBTree<_MapConfig<_Key, _Val, _Compare, Alloc, true>>
	{
		using _Config = _MapConfig<_Key, _Val, _Compare, Alloc, true>;
		using _Base = _RBTree<_Config>;
	public:
		using DifferenceType = typename _Config::DifferenceType;
		using ConstReference = typename _Config::ConstReference;
		using ConstPointer = typename _Config::ConstPointer;
		using ValueType = typename _Config::ValueType;
		using KeyType = typename _Config::KeyType;
		using MappedType = typename _Config::MappedType;
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
		MultiMap() :
			_Base()
		{}

		explicit MultiMap(const KeyCompare &comp,
			const AllocatorType &alloc = AllocatorType()) :
			_Base(comp, alloc)
		{}

		template<typename InputIt>
		MultiMap(InputIt first, InputIt last,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(first, last, comp, alloc)
		{}

		MultiMap(const MultiMap &that) :
			_Base(that)
		{}

		MultiMap(MultiMap &&that) noexcept :
			_Base(MSTD::move(that))
		{}

		MultiMap(std::initializer_list<ValueType> il,
			const KeyCompare &comp = KeyCompare(),
			const AllocatorType &alloc = AllocatorType()) :
			_Base(il, comp, alloc)
		{}

		MultiMap& operator=(const MultiMap &that)
		{
			_Base::operator=(that);
			return *this;
		}

		MultiMap& operator=(MultiMap &&that) noexcept
		{
			_Base::operator=(MSTD::move(that));
			return *this;
		}

		MultiMap& operator=(std::initializer_list<ValueType> il)
		{
			_Base::operator=(il);
			return *this;
		}

		/////////////////////////////////////
		//
		//			Element access
		//
		/////////////////////////////////////

		MappedType& at(const KeyType &key)
		{
			Iterator pos = _Base::find(key);
			if (pos != _Base::end()) {
				return (*pos).second;
			}
			else {
				throw std::out_of_range("No such key in map");
			}
		}

		const MappedType& at(const KeyType &key) const
		{
			Iterator pos = _Base::find(key);
			if (pos != _Base::end()) {
				return (*pos).second;
			}
			else {
				throw std::out_of_range("No such key in map");
			}
		}

		MappedType& operator[](const KeyType &key)
		{
			auto pos = _Base::find(key);
			if (pos != _Base::end()) {
				return pos->second;
			}
			else {
				auto ret = _Base::insert(ValueType(key, MappedType()));
				return ret.first->second;
			}
		}
	};

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator==(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator!=(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator<(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator<=(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
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

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator>(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	bool operator>=(const MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
					const MultiMap<_Key, _Val, _Compare, _Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename _Key, typename _Val, typename _Compare, typename _Alloc>
	void swap(MultiMap<_Key, _Val, _Compare, _Alloc> &lhs,
				MultiMap<_Key, _Val, _Compare, _Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}
}