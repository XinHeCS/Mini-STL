#pragma once

// Stack adaptor standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>
#include <Container/Deque.h>

// STL header and cpp standard header

namespace MSTD {

	template<typename T, typename Container = Deque<T>>
	class Stack
	{
	public:
		using ContainerType = Container;
		using SizeType = typename Container::SizeType;
		using Reference = typename Container::Reference;
		using ValueType = typename Container::ValueType;
		using ConstReference = typename Container::ConstReference;

		static_assert(
			isSame<T, ValueType>::value,
			"Adaptor's elements should be identical to Container's."
		);

		Stack() :
			Stack(Container())
		{}

		explicit Stack(const Container &con) :
			_c(con)
		{}

		explicit Stack(Container &&con) :
			_c(move(con))
		{}

		Stack(const Stack &that) :
			_c(that._c)
		{}

		Stack(Stack &&that) noexcept:
			_c(move(that._c))
		{}

		template<typename Alloc>
		Stack(const Alloc &alloc) :
			_c(alloc)
		{}

		template<typename Alloc>
		Stack(const Container &con, const Alloc &alloc) :
			_c(con, alloc)
		{}

		template<typename Alloc>
		Stack(Container &&con, const Alloc &alloc) :
			_c(move(con), alloc)
		{}

		template<typename Alloc>
		Stack(const Stack &that, const Alloc &alloc) :
			_c(that._c, alloc)
		{}

		template<typename Alloc>
		Stack(Stack &&that, const Alloc &alloc) :
			_c(move(that._c), alloc)
		{}

		Stack& operator=(const Stack &that)
		{
			if (this != addressof(that)) {
				_c = that._c;
			}
			return *this;
		}

		Stack& operator=(Stack &&that) noexcept
		{
			if (this != addressof(that)) {
				_c = move(that._c);
			}
			return *this;
		}

		Reference top()
		{
			return _c.back();
		}

		ConstReference top() const
		{
			return _c.back();
		}

		bool empty() const
		{
			return _c.empty();
		}

		SizeType size() const
		{
			return _c.size();
		}

		void push(const ValueType &val)
		{
			_c.pushBack(val);
		}

		void push(ValueType &&val)
		{
			_c.pushBack(move(val));
		}

		template<typename... Args>
		void emplace(Args&&... args)
		{
			_c.emplaceBack(forward<Args>(args)...);
		}

		void pop()
		{
			_c.popBack();
		}

		void swap(Stack &that) noexcept
		{
			_c.swap(that._c);
		}
		
		bool operator==(const Stack &that)
		{
			return _c == that._c;
		}

		bool operator!=(const Stack &that)
		{
			return _c != that._c;
		}

		bool operator<(const Stack &that)
		{
			return _c < that._c;
		}

		bool operator<=(const Stack &that)
		{
			return _c <= that._c;
		}

		bool operator>(const Stack &that)
		{
			return _c > that._c;
		}

		bool operator>=(const Stack &that)
		{
			return _c >= that._c;
		}

	protected:
		Container _c;
	};

	template<class T, class Container>
	void swap(Stack<T, Container>& lhs, Stack<T, Container>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

}