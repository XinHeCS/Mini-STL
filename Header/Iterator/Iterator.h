#pragma once

// Iterator standard header

// Mini-STL header
#include <Config/Config.h>
#include <TypeInfo/TypeHelper.h>
#include <Utility/addressof.h>
#include <Utility/move.h>

namespace MSTD {

	// Empty class types used to indicate iterator categories 
	struct InputIteratorTag {};
	struct OutputIteratorTag {};
	struct ForwardIteratorTag : InputIteratorTag, OutputIteratorTag {};
	struct BidirectionalIteratorTag : ForwardIteratorTag {};
	struct RandomAccessIteratorTag : BidirectionalIteratorTag {};

	// Register types for classes.
	// Classes can check the existence of 
	// these types of some certain class register here.

	template<typename Iter>
	struct IteratorTraits
	{
	private:
		REGISTER_TYPE_TRAIT(IteratorCategory);
		REGISTER_TYPE_TRAIT(DifferenceType);
		REGISTER_TYPE_TRAIT(ValueType);
		REGISTER_TYPE_TRAIT(Reference);
		REGISTER_TYPE_TRAIT(Pointer);

	public:
		using IteratorCategory = typename _IteratorCategory_Trait_<Iter>::type;

		using DifferenceType = typename _DifferenceType_Trait_<Iter>::type;

		using ValueType = typename _ValueType_Trait_<Iter>::type;

		using Reference = typename _Reference_Trait_<Iter>::type;

		using Pointer = typename _Pointer_Trait_<Iter>::type;
	};

	template<typename T>
	struct IteratorTraits<T*>
	{
		using IteratorCategory = RandomAccessIteratorTag;

		using DifferenceType = ptrdiff_t;

		using ValueType = T;

		using Reference = T & ;

		using Pointer = T * ;
	};

	template<typename T>
	struct IteratorTraits<const T*>
	{
		using IteratorCategory = RandomAccessIteratorTag;

		using DifferenceType = ptrdiff_t;

		using ValueType = T;

		using Reference = const T & ;

		using Pointer = const T * ;
	};


	// Iterator adaptors

	template<typename Iter>
	class ReverseIterator
	{
	public:
		using IteratorCategory = typename IteratorTraits<Iter>::IteratorCategory;
		using DifferenceType = typename IteratorTraits<Iter>::DifferenceType;
		using ValueType = typename IteratorTraits<Iter>::ValueType;
		using Reference = typename IteratorTraits<Iter>::Reference;
		using Pointer = typename IteratorTraits<Iter>::Pointer;

		ReverseIterator() :
			_current()
		{}

		explicit ReverseIterator(Iter that) :
			_current(that)
		{}

		template<typename OtherIter>
		ReverseIterator(const ReverseIterator<OtherIter> &that) :
			_current(that.base())
		{}

		template<typename OtherIter>
		ReverseIterator& operator=(const ReverseIterator<OtherIter> &that)
		{
			_current = that.base();
			return *this;
		}

		Iter base() const { return _current; }

		Reference operator*() const
		{
			auto tmp = _current;
			return *--tmp;
		}

		Pointer operator->() const
		{
			return addressof(this->operator*());
		}

		ReverseIterator& operator++()
		{
			--_current;
			return *this;
		}

		ReverseIterator operator++(int)
		{
			auto tmp = _current;
			--_current;
			return ReverseIterator(tmp);
		}

		ReverseIterator& operator--()
		{
			++_current;
			return &this;
		}

		ReverseIterator operator--(int)
		{
			auto tmp = _current;
			++_current;
			return ReverseIterator(tmp);
		}

		ReverseIterator& operator+=(DifferenceType diff)
		{
			_current -= diff;
			return *this;
		}

		ReverseIterator operator+(DifferenceType diff) const
		{
			return ReverseIterator(_current - diff);
		}

		ReverseIterator& operator-=(DifferenceType diff)
		{
			_current += diff;
			return *this;
		}

		ReverseIterator operator-(DifferenceType diff) const
		{
			return ReverseIterator(_current + diff);
		}

		Reference operator[](DifferenceType diff) const
		{
			return *(*this + diff);
		}

	protected:
		// Base iterator
		Iter _current;
	};

	// Compare operators for reverse iterator
	template<typename Iter, typename OtherIter>
	bool operator==(const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator!=(const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Iter, typename OtherIter>
	bool operator<(	const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return rhs.base() < lhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator<=(const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return rhs.base() <= lhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator>( const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return rhs.base() > lhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator>=(const ReverseIterator<Iter> &lhs,
					const ReverseIterator<OtherIter> &rhs)
	{
		return rhs.base() >= lhs.base();
	}

	template<typename Iter>
	ReverseIterator<Iter> operator+(typename ReverseIterator<Iter>::DifferenceType diff,
									const ReverseIterator<Iter> &it)
	{
		return it + diff;
	}

	template<typename Iter>
	auto operator-(	const ReverseIterator<Iter> &lhs,
					const ReverseIterator<Iter> &rhs)
		-> decltype(rhs.base() - lhs.base())
	{
		return rhs.base() - lhs.base();
	}


	template<typename Iter>
	class MoveIterator
	{
	public:
		using IteratorCategory = typename IteratorTraits<Iter>::IteratorCategory;
		using DifferenceType = typename IteratorTraits<Iter>::DifferenceType;
		using ValueType = typename IteratorTraits<Iter>::ValueType;
		using Reference = ValueType && ;
		using Pointer = Iter;

		MoveIterator() :
			_current()
		{}	

		explicit MoveIterator(Iter x) :
			_current(x)
		{}	

		template<typename OtherIter>
		MoveIterator(const MoveIterator<OtherIter> &that) :
			_current(that.base())
		{}

		template<typename OtherIter>
		MoveIterator& operator=(const MoveIterator<OtherIter> &that)
		{
			_current = that.base();
			return *this;
		}

		Iter base() const { return _current; }

		Reference operator*() const
		{
			return move(*_current);
		}

		Pointer operator->() const
		{
			return _current;
		}

		MoveIterator& operator++()
		{
			++_current;
			return *this;
		}

		MoveIterator operator++(int)
		{
			auto tmp = _current;
			++_current;
			return MoveIterator(tmp);
		}

		MoveIterator& operator--()
		{
			--_current;
			return *this;
		}

		MoveIterator operator--(int)
		{
			auto tmp = _current;
			--_current;
			return MoveIterator(tmp);
		}

		MoveIterator& operator+=(DifferenceType diff)
		{
			_current += diff;
			return *this;
		}

		MoveIterator operator+(DifferenceType diff) const
		{
			return MoveIterator(_current + diff);
		}

		MoveIterator& operator-=(DifferenceType diff)
		{
			_current -= diff;
			return *this;
		}

		MoveIterator operator-(DifferenceType diff) const
		{
			return MoveIterator(_current - diff);
		}

		Reference operator[](DifferenceType diff) const
		{
			return move(*(_current + diff));
		}

	private:
		Iter _current;
	};

	// Compare operators for move iterator
	template<typename Iter, typename OtherIter>
	bool operator==(const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator!=(const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Iter, typename OtherIter>
	bool operator<(	const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator<=(const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator>(	const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return lhs.base() > rhs.base();
	}

	template<typename Iter, typename OtherIter>
	bool operator>=(const MoveIterator<Iter> &lhs,
					const MoveIterator<OtherIter> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template<typename Iter>
	MoveIterator<Iter> operator+(typename MoveIterator<Iter>::DifferenceType diff,
								const MoveIterator<Iter> &it)
	{
		return it + diff;
	}

	template<typename Iter>
	auto operator-(	const MoveIterator<Iter> &lhs,
					const MoveIterator<Iter> &rhs)
		-> decltype(lhs.base() - rhs.base())
	{
		return lhs.base() - rhs.base();
	}

	// Iterator adaptors useful functions
	template<typename Iter>
	MoveIterator<Iter> makeMoveIterator(Iter it)
	{
		return MoveIterator<Iter>(it);
	}

	template<typename Container>
	class BackInsertIterator
	{
	public:
		using IteratorCategory = OutputIteratorTag;
		using ContainerType = Container;
		using DifferenceType = void;
		using ValueType = void;
		using Reference = void;
		using Pointer = void;

		explicit BackInsertIterator(Container &c) :
			_container(&c)
		{}

		BackInsertIterator&
			operator=(typename Container::ValueType& val)
		{
			_container->pushBack(val);
			return *this;
		}

		BackInsertIterator&
			operator=(typename Container::ValueType&& val)
		{
			_container->pushBack(move(val));
			return *this;
		}

		BackInsertIterator& operator*()
		{
			return *this;
		}

		BackInsertIterator& operator++()
		{
			return *this;
		}

		BackInsertIterator& operator++(int)
		{
			return *this;
		}

	private:
		Container *_container;
	};

	template<typename Container>
	BackInsertIterator<Container> backInserter(Container &c)
	{
		return BackInsertIterator<Container>(c);
	}

	template<typename Container>
	class FrontInsertIterator
	{
	public:
		using IteratorCategory = OutputIteratorTag;
		using ContainerType = Container;
		using DifferenceType = void;
		using ValueType = void;
		using Reference = void;
		using Pointer = void;

		explicit FrontInsertIterator(Container &c) :
			_container(&c)
		{}

		FrontInsertIterator&
			operator=(typename Container::ValueType& val)
		{
			_container->pushFront(val);
			return *this;
		}

		FrontInsertIterator&
			operator=(typename Container::ValueType&& val)
		{
			_container->pushFront(move(val));
			return *this;
		}

		FrontInsertIterator& operator*()
		{
			return *this;
		}

		FrontInsertIterator& operator++()
		{
			return *this;
		}

		FrontInsertIterator& operator++(int)
		{
			return *this;
		}

	private:
		Container *_container;
	};

	template<typename Container>
	FrontInsertIterator<Container> frontInserter(Container &c)
	{
		return FrontInsertIterator<Container>(c);
	}

	template<typename Container>
	class InsertIterator
	{
	public:
		using IteratorCategory = OutputIteratorTag;
		using ContainerType = Container;
		using DifferenceType = void;
		using ValueType = void;
		using Reference = void;
		using Pointer = void;

		InsertIterator(Container &c, typename Container::iterator it) :
			_container(&c),
			_iter(it)
		{}

		InsertIterator&
			operator=(typename Container::ValueType& val)
		{
			_iter = _container->insert(_iter, val);
			++_iter;
			return *this;
		}

		InsertIterator&
			operator=(typename Container::ValueType&& val)
		{
			_iter = _container->insert(_iter, move(val));
			++_iter;
			return *this;
		}

		InsertIterator& operator*()
		{
			return *this;
		}

		InsertIterator& operator++()
		{
			return *this;
		}

		InsertIterator& operator++(int)
		{
			return *this;
		}

	private:
		Container *_container;
		typename Container::iterator _iter;
	};

	template<typename Container>
	InsertIterator<Container> inserter(Container &c)
	{
		return InsertIterator<Container>(c);
	}

	// Iterator operations

	// advance
	template<typename Iter, typename Distance>
	void _aux_Advance(Iter &it, Distance n)
	{
		static_assert(false, "Invalid interator type");
	}

	template<typename Iter, typename Distance>
	void _aux_Advance(Iter &it, Distance n, InputIteratorTag)
	{
		static_assert(n >= 0, "Can't decrease an input iterator");
		while (n--) ++it;
	}

	template<typename Iter, typename Distance>
	void _aux_Advance(Iter &it, Distance n, BidirectionalIteratorTag)
	{
		if (n >= 0) {
			while (n--) ++it;
		}
		else {
			while (n++) --it;
		}
	}

	template<typename Iter, typename Distance>
	void _aux_Advance(Iter &it, Distance n, RandomAccessIteratorTag)
	{
		it += n;
	}

	template<typename Iter, typename Distance>
	void advance(Iter &it, Distance n)
	{
		_aux_Advance(it, n, typename IteratorTraits<Iter>::IteratorCategory());
	}

	// distance
	template<typename Iter>
	typename IteratorTraits<Iter>::DifferenceType
		_aux_Distance(Iter first, Iter second)
	{
		static_assert(false, "Invalid interator type");
		return 0;
	}

	template<typename Iter>
	typename IteratorTraits<Iter>::DifferenceType
		_aux_Distance(Iter first, Iter second, InputIteratorTag)
	{
		typename IteratorTraits<Iter>::DifferenceType diff = 0;
		for (; first != second; ++first) ++diff;

		return diff;
	}

	template<typename Iter>
	typename IteratorTraits<Iter>::DifferenceType
		_aux_Distance(Iter first, Iter second, RandomAccessIteratorTag)
	{
		return second - first;
	}

	template<typename Iter>
	typename IteratorTraits<Iter>::DifferenceType
		distance(Iter first, Iter second)
	{
		return _aux_Distance(first, second, typename IteratorTraits<Iter>::IteratorCategory());
	}

	// next
	template<typename Iter>
	Iter next(Iter it, typename IteratorTraits<Iter>::DifferenceType n = 1)
	{
		advance(it, n);
		return it;
	}

	// prev
	template<typename Iter>
	Iter prev(Iter it, typename IteratorTraits<Iter>::DifferenceType n = 1)
	{
		advance(it, -n);
		return it;
	}

	// begin
	template<typename Container>
	auto begin(Container &c) -> decltype(c.begin())
	{
		return c.begin();
	}

	template<typename Container>
	auto begin(const Container &c) -> decltype(c.begin())
	{
		return c.begin();
	}

	template<typename T, size_t N>
	T* begin(T (&arr)[N])
	{
		return arr;
	}

	// end
	template<typename Container>
	auto end(Container &c) -> decltype(c.end())
	{
		return c.end();
	}

	template<typename Container>
	auto end(const Container &c) -> decltype(c.end())
	{
		return c.end();
	}

	template<typename T, size_t N>
	T* end(T(&arr)[N])
	{
		return arr + N;
	}
}