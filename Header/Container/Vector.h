#pragma once

// Vector standard header

// Mini-STL header
#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>

// STL header and cpp standard header
#include <initializer_list>
#include <stdexcept>

namespace MSTD {

	// Iterator of vector
	template<typename Vec>
	class _VecIterator
	{
	public:
		using IteratorCategory = RandomAccessIteratorTag;
		using DifferenceType = typename Vec::DifferenceType;
		using ValueType = typename Vec::ValueType;
		using Pointer = typename Vec::Pointer;
		using Reference = typename Vec::Reference;

		_VecIterator() :
			_ptr(nullptr)
		{}

		_VecIterator(Pointer ptr) :
			_ptr(ptr)
		{}

		Pointer base() const { return _ptr; }

		Reference operator*()
		{
			return *_ptr;
		}

		Pointer operator->()
		{
			return base();
		}

		_VecIterator& operator++()
		{
			++_ptr;
			return *this;
		}

		_VecIterator operator++(int)
		{
			auto tmp = _ptr;
			++_ptr;
			return _VecIterator(tmp);
		}

		_VecIterator& operator--()
		{
			--_ptr;
			return *this;
		}

		_VecIterator operator--(int)
		{
			auto tmp = _ptr;
			--_ptr;
			return _VecIterator(tmp);
		}

		_VecIterator& operator+=(DifferenceType diff)
		{
			_ptr += diff;
			return *this;
		}

		_VecIterator operator+(DifferenceType diff)
		{
			return _VecIterator(_ptr + diff);
		}

		_VecIterator& operator-=(DifferenceType diff)
		{
			_ptr -= diff;
			return *this;
		}

		_VecIterator operator-(DifferenceType diff)
		{
			return _VecIterator(_ptr - diff);
		}

		Reference operator[](DifferenceType diff)
		{
			return *(_ptr + diff);
		}
		
	protected:
		Pointer _ptr;
	};

	// Compare operators of vector::iterator
	template<typename Vec>
	bool operator==(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Vec>
	bool operator!=(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return !(lhs.base() == rhs.base());
	}

	template<typename Vec>
	bool operator<(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Vec>
	bool operator<=(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template<typename Vec>
	bool operator>(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() > rhs.base();
	}

	template<typename Vec>
	bool operator>=(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template<typename Vec>
	typename _VecIterator<Vec>::DifferenceType 
		operator-(const _VecIterator<Vec> &lhs,
					const _VecIterator<Vec> &rhs)
	{
		return lhs.base() - rhs.base();
	}

	template<typename Vec>
	_VecIterator<Vec>
		operator+(typename _VecIterator<Vec>::DifferenceType diff,
			const _VecIterator<Vec> &iter)
	{
		return iter + diff;
	}

	// Const Iterator of vector<T>
	template<typename Vec>
	class _VecConstIterator
	{
	public:
		using IteratorCategory = RandomAccessIteratorTag;
		using DifferenceType = typename Vec::DifferenceType;
		using ValueType = typename Vec::ValueType;
		using ConstPointer = typename Vec::ConstPointer;
		using Pointer = typename Vec::Pointer;
		using Reference = typename Vec::Reference;
		using ConstReference = typename Vec::ConstReference;

		_VecConstIterator() :
			_ptr(nullptr)
		{}

		_VecConstIterator(Pointer ptr) :
			_ptr(ptr)
		{}

		_VecConstIterator(const _VecIterator<Vec> &other) :
			_ptr(other.base())
		{}

		Pointer base() const { return _ptr; }

		ConstReference operator*()
		{
			return *_ptr;
		}

		ConstPointer operator->()
		{
			return base();
		}

		_VecConstIterator& operator++()
		{
			++_ptr;
			return *this;
		}

		_VecConstIterator operator++(int)
		{
			auto tmp = _ptr;
			++_ptr;
			return _VecConstIterator(tmp);
		}

		_VecConstIterator& operator--()
		{
			--_ptr;
			return *this;
		}

		_VecConstIterator operator--(int)
		{
			auto tmp = _ptr;
			--_ptr;
			return _VecConstIterator(tmp);
		}

		_VecConstIterator& operator+=(DifferenceType diff)
		{
			_ptr += diff;
			return *this;
		}

		_VecConstIterator operator+(DifferenceType diff)
		{
			return _VecConstIterator(_ptr + diff);
		}

		_VecConstIterator& operator-=(DifferenceType diff)
		{
			_ptr -= diff;
			return *this;
		}

		_VecConstIterator operator-(DifferenceType diff)
		{
			return _VecConstIterator(_ptr - diff);
		}

		ConstReference operator[](DifferenceType diff)
		{
			return *(_ptr + diff);
		}

	protected:
		Pointer _ptr;
	};

	// Compare operators of vector::ConstIterator
	template<typename Vec>
	bool operator==(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Vec>
	bool operator!=(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return !(lhs.base() == rhs.base());
	}

	template<typename Vec>
	bool operator<(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Vec>
	bool operator<=(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() <= rhs.base();
	}

	template<typename Vec>
	bool operator>(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() > rhs.base();
	}

	template<typename Vec>
	bool operator>=(const _VecConstIterator<Vec> &lhs,
					const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() >= rhs.base();
	}

	template<typename Vec>
	typename _VecConstIterator<Vec>::DifferenceType
		operator-(const _VecConstIterator<Vec> &lhs,
				const _VecConstIterator<Vec> &rhs)
	{
		return lhs.base() - rhs.base();
	}

	template<typename Vec>
	_VecConstIterator<Vec>
		operator+(typename _VecConstIterator<Vec>::DifferenceType diff,
				const _VecConstIterator<Vec> &iter)
	{
		return iter + diff;
	}

	// Wrapper Vector<T>'s 
	// internal Pointer
	template<typename Pointer>
	class _VecBase
	{
	public:
		_VecBase() noexcept :
			_beg(nullptr),
			_end(nullptr),
			_capacity(nullptr)
		{}

		_VecBase(Pointer beg, Pointer end, Pointer cap) noexcept :
			_beg(beg),
			_end(end),
			_capacity(cap)
		{}

		_VecBase(const _VecBase &that) noexcept :
			_beg(that._beg),
			_end(that._end),
			_capacity(that._capacity)
		{}

		_VecBase(_VecBase &&that) noexcept :
			_beg(move(that._beg)),
			_end(move(that._end)),
			_capacity(move(that._capacity))
		{
			that._beg = that._end = that._capacity = nullptr;
		}

		_VecBase& operator=(const _VecBase &that)
		{
			if (&that != this) {
				_beg = that._beg;
				_end = that._end;
				_capacity = that._capacity;
			}

			return *this;
		}

		_VecBase& operator=(_VecBase &&that) noexcept
		{
			_beg = move(that._beg);
			_end = move(that._end);
			_capacity = move(that._capacity);

			that._beg = that._end = that._capacity = nullptr;

			return *this;
		}

		void swap(_VecBase &that) noexcept
		{
			using std::swap;
			swap(_beg, that._beg);
			swap(_end, that._end);
			swap(_capacity, that._capacity);
		}

		Pointer _beg;
		Pointer _end;
		Pointer _capacity;
	};

	// Vector<T>
	// Vector<T> is a sequence container that encapsulates 
	// dynamic size arrays.
	template<
		typename T, // Element type
		typename Alloc = Allocator<T> // Standard allocator
	> class Vector
	{
	public:
		using AllocatorType = Alloc;
		using DifferenceType = ptrdiff_t;
		using SizeType = size_t;
		using ValueType = T;
		using Reference = ValueType & ;
		using ConstReference = const ValueType&;
		using Pointer = typename AllocatorTraits<Alloc>::Pointer;
		using ConstPointer = typename AllocatorTraits<Alloc>::ConstPointer;
		using Iterator = _VecIterator<Vector>;
		using ConstIterator = _VecConstIterator<Vector>;
		using ConstReverseIterator = ReverseIterator<ConstIterator>;
		using ReverseIterator = ReverseIterator<Iterator>;

		static_assert(isSame<ValueType, typename AllocatorTraits<Alloc>::ValueType>::value,
					  "Allocator require the same type T with Vector<T>");

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		/////////////////////////////////////

		Vector() noexcept(noexcept(Alloc())) :
			_vBase(),
			_alloc()
		{}

		explicit Vector(const Alloc &alloc) noexcept :
			_vBase(),
			_alloc(alloc)
		{}

		Vector(SizeType count, const ValueType &val, const Alloc &alloc = Alloc()) :
			Vector(alloc)
		{
			_MSTD_TRY
				if (count < _alloc.maxSize()) {
					_aux_insertRange(_vBase._beg, count, val);
				}
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		explicit Vector(SizeType count, const Alloc &alloc = Alloc()) :
			Vector(alloc)
		{
			_MSTD_TRY
				if (count < _alloc.maxSize()) {
					_vBase = _reallocate(_getGrownCapacity(count));
				}
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		template<
			typename InputIt, 
			typename = typename enableIf<
								!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value,
								void
			>::type>
		Vector(InputIt first, InputIt last, const Alloc &alloc = Alloc()) :
			Vector(alloc)
		{
			_MSTD_TRY
				_aux_insertRange(_vBase._beg, first, last);
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		Vector(const Vector &that) :
			Vector(Alloc())
		{
			_MSTD_TRY
				_aux_insertRange(_vBase._beg, that.begin(), that.end());
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		Vector(const Vector &that, const Alloc &alloc) :
			Vector(alloc)
		{
			_MSTD_TRY
				_aux_insertRange(_vBase._beg, that.begin(), that.end());
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		Vector(Vector &&that) noexcept :
			_vBase(move(that._vBase)),
			_alloc(move(that._alloc))
		{}

		Vector(Vector &&that, const Alloc &alloc) 
			try :
			_vBase(move(that._vBase)),
			_alloc(alloc)
		{}
		catch (...) { throw; }

		Vector(std::initializer_list<ValueType> li, const Alloc &alloc = Alloc()) :
			Vector(alloc)
		{
			_MSTD_TRY
				_aux_insertRange(_vBase._beg, li.begin(), li.end());
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		~Vector() noexcept
		{
			_destroy();
		}

		AllocatorType getAllocator() const
		{
			return _alloc;
		}

		Vector& operator=(const Vector &that)
		{
			if (this != &that) {
				_destroy();
				_MSTD_TRY
					_alloc = that._alloc;
					_aux_insertRange(_vBase._beg, that.begin(), that.end());
				_MSTD_CATCH_ALL
					_destroy();
					throw;
				_MSTD_END_CATCH
			}

			return *this;
		}

		Vector& operator=(Vector &&that) noexcept
		{
			if (this != &that ) {
				_vBase = move(that._vBase);
				_alloc = move(that._alloc);
			}
			
			return *this;
		}

		Vector& operator=(std::initializer_list<ValueType> li)
		{
			_MSTD_TRY
				_alloc = AllocatorType();
				_aux_insertRange(_vBase._beg, li.begin(), li.end());
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH

			return *this;
		}

		void assign(SizeType count, const ValueType &val)
		{
			_MSTD_TRY
				_destroy();
				if (count < _alloc.maxSize()) {
					_aux_insertRange(_vBase._beg, count, val);
				}
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}

		template<
			typename InputIt,
			typename = typename enableIf<
			!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value,
			void
		>::type>
			void assign(InputIt first, InputIt last)
		{
			_MSTD_TRY
				_destroy();
				_aux_insertRange(_vBase._beg, first, last);
			_MSTD_CATCH_ALL
				_destroy();
				throw;
			_MSTD_END_CATCH
		}


		/////////////////////////////////////
		//
		//			 Element access
		//
		/////////////////////////////////////

		Reference at(SizeType pos)
		{
			if (!_verifyRange(pos)) {
				throw std::out_of_range("Invalid index to access");
			}
			return *(_vBase._beg + pos);
		}

		ConstReference at(SizeType pos) const
		{
			if (!_verifyRange(pos)) {
				throw std::out_of_range("Invalid index to access");
			}
			return *(_vBase._beg + pos);
		}

		Reference operator[](SizeType pos)
		{
			return *(_vBase._beg + pos);
		}

		ConstReference operator[](SizeType pos) const
		{
			return *(_vBase._beg + pos);
		}

		Reference front()
		{
			return *(_vBase._beg);
		}

		ConstReference front() const
		{
			return *(_vBase._beg);
		}

		Reference back()
		{
			return *(_vBase._end - 1);
		}

		ConstReference back() const
		{
			return *(_vBase._end - 1);
		}

		Pointer data()
		{
			return _vBase._beg;
		}

		ConstPointer data() const
		{
			return _vBase._beg;
		}

		/////////////////////////////////////
		//
		//			 Iterators
		//
		/////////////////////////////////////	

		// begin and cbegin
		Iterator begin() noexcept
		{
			return Iterator(_vBase._beg);
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(_vBase._beg);
		}

		ConstIterator cbegin() const noexcept
		{
			return ConstIterator(_vBase._beg);
		}

		// end and cend
		Iterator end() noexcept
		{
			return Iterator(_vBase._end);
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(_vBase._end);
		}

		ConstIterator cend() const noexcept
		{
			return ConstIterator(_vBase._end);
		}

		// rbegin and crbegin
		ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(_vBase._end);
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(_vBase._end);
		}

		ConstReverseIterator crbegin() const noexcept
		{
			return ConstReverseIterator(_vBase._end);
		}

		// rend and crend
		ReverseIterator rend() noexcept
		{
			return ReverseIterator(_vBase._beg);
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(_vBase._beg);
		}

		ConstReverseIterator crend() const noexcept
		{
			return ConstReverseIterator(_vBase._beg);
		}
		
		/////////////////////////////////////
		//
		//			 Capacity
		//
		/////////////////////////////////////		

		bool empty() const noexcept
		{
			return _vBase._beg == _vBase._end;
		}

		SizeType size() const noexcept
		{
			return _vBase._end - _vBase._beg;
		}

		SizeType maxSize() const noexcept
		{
			return _alloc.maxSize();
		}

		void reserve(SizeType newCap)
		{
			if (newCap > capacity()) {
				_VecBase<Pointer> newBase;
				_MSTD_TRY
					newBase = _reallocate(newCap);
					newBase._end = _moveRange(_vBase._beg, _vBase._end, newBase._beg);		

					_destroy();
					_vBase.swap(newBase);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH
			}
		}

		SizeType capacity() const noexcept
		{
			return _vBase._capacity - _vBase._beg;
		}

		void shrinkToFit()
		{
			if (size() < capacity()) {
				auto newSize = size();
				_VecBase<Pointer> newBase;
				_MSTD_TRY
					newBase = _reallocate(newSize);
					newBase._end = _moveRange(_vBase._beg, _vBase._end, newBase._beg);

					_destroy();
					_vBase.swap(newBase);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH
			}
		}

		/////////////////////////////////////
		//
		//			 Modifiers
		//
		/////////////////////////////////////

		void clear()
		{
			auto cap = capacity();
			_destroy();
			_vBase = _reallocate(cap);
		}

		Iterator insert(ConstIterator iter, const ValueType &val)
		{
			return Iterator(_aux_insert(iter.base(), val));
		}

		Iterator insert(ConstIterator iter, ValueType &&val)
		{
			return Iterator(_aux_insert(iter.base(), move(val)));
		}

		Iterator insert(ConstIterator iter, SizeType count, const ValueType &val)
		{
			return Iterator(_aux_insertRange(iter.base(), count, val));
		}

		template<
			typename InputIt,
			typename = typename enableIf<
				!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value,
				void>::type
		>
		Iterator insert(ConstIterator iter, InputIt first, InputIt last)
		{
			return Iterator(_aux_insertRange(iter.base(), first, last));
		}

		Iterator insert(ConstIterator iter, std::initializer_list<ValueType> li)
		{
			return Iterator(_aux_insertRange(iter.base(), li.begin(), li.end()));
		}

		template<typename... Args>
		Iterator emplace(ConstIterator pos, Args&&... args)
		{
			Pointer rawPos = pos.base();
			if (_hasEnoughCapacity(1)) {
				if (rawPos == _vBase._end) {
					_alloc.construct(rawPos, forward<Args>(args)...);
					++_vBase._end;
				}
				else {
					_copyBackward(rawPos, _vBase._end, 1);
					_alloc.construct(rawPos, forward<Args>(args)...);
				}
				return Iterator(pos.base());
			}
			else {
				// Reallocate
				Pointer ret;
				_VecBase<Pointer> newBase;
				_MSTD_TRY
					newBase = _reallocate(_getGrownCapacity(1));
					newBase._end = _moveRange(_vBase._beg, rawPos, newBase._beg);

					ret = newBase._end;
					_alloc.construct(newBase._end, forward<Args>(args)...);
					++newBase._end;

					newBase._end = _moveRange(rawPos, _vBase._end, newBase._end);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH

				_destroy();
				_vBase.swap(newBase);
				return Iterator(ret);
			}
		}

		Iterator erase(ConstIterator pos)
		{			
			if (!empty()) {
				Pointer rawPos = pos.base();
				_alloc.destroy(rawPos);
				_copyBackward(rawPos + 1, _vBase._end, -1);
			}
			return Iterator(pos.base());
		}

		Iterator erase(ConstIterator first, ConstIterator last)
		{
			if (!empty()) {
				Pointer rawFirst = first.base();
				Pointer rawLast = last.base();
				for (; rawFirst != rawLast; ++rawFirst) {
					_alloc.destroy(rawFirst);
				}
				_copyBackward(rawLast, _vBase._end, first - last);
			}

			return Iterator(last.base());
		}

		void pushBack(const ValueType &val)
		{
			emplaceBack(val);
		}

		void pushBack(ValueType &&val)
		{
			emplaceBack(move(val));
		}

		template<typename... Args>
		void emplaceBack(Args&&... args)
		{
			if (_hasEnoughCapacity(1)) {
				_alloc.construct(_vBase._end, forward<Args>(args)...);
				++_vBase._end;
			}
			else {
				_VecBase<Pointer> newBase;
				_MSTD_TRY
					newBase = _reallocate(_getGrownCapacity(1));
					newBase._end = _moveRange(_vBase._beg, _vBase._end, newBase._beg);
					_alloc.construct(newBase._end, forward<Args>(args)...);
					++newBase._end;

					_destroy();
					_vBase.swap(newBase);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH
			}
		}

		void popBack()
		{
			_alloc.destroy(_vBase._end);
			--_vBase._end;
		}

		void resize(SizeType count)
		{
			_aux_resize(count);
		}

		void resize(SizeType count, const ValueType& val)
		{
			_aux_resize(count, val);
		}

		void swap(Vector &that) noexcept
		{
			_vBase.swap(that._vBase);
			using std::swap;
			swap(_alloc, that._alloc);
		}

	protected:
		_VecBase<Pointer> _vBase;
		Alloc _alloc;

	private:

		/////////////////////////////////////
		//
		//		private utilities
		//
		/////////////////////////////////////

		// Copy elements from range [start, end) backward {range} position
		// [start, end) -> [start + range, end + range)
		void _copyBackward(Pointer first, Pointer last, DifferenceType range)
		{
			//for (auto it = last; it > first; --it) {
			//	::new (static_cast<void*>(it + range)) ValueType(move(*it));
			//}
			std::memmove(first + range, first, (last - first) * sizeof(ValueType));
			_vBase._end += range;
		}

		// If throw except move construct
		Pointer _moveNoexcept(Pointer first, Pointer last, Pointer dest, falseType)
		{
			// Copy construct
			return uninitializedCopy(
				first,
				last,
				dest
			);
		}

		// If nothrow move construct
		Pointer _moveNoexcept(Pointer first, Pointer last, Pointer dest, trueType)
		{
			// Move construct
			return uninitializedCopy(
				MoveIterator<Pointer>(first),
				MoveIterator<Pointer>(last),
				dest
			);
		}

		// Try to move elements to {dest} from range [start, end)
		Pointer _moveRange(Pointer start, Pointer end, Pointer dest)
		{
			return _moveNoexcept(
				start, end, dest,
				typename conditional<isNothrowMoveConstructible<ValueType>::value, 
									trueType, falseType>::type()
			);
		}

		// Provide strong guarantee
		Pointer _aux_insertRange(Pointer pos, SizeType count, const ValueType &val)
		{				
			if (_hasEnoughCapacity(count)) {
				// needn't allocate new sapce,
				// Standard insert
				ValueType valCopy = val;
				_copyBackward(pos, _vBase._end, count);
				uninitializedFill_N(pos, count, valCopy);
				return pos;
			}
			else {
				_VecBase<Pointer> newBase;
				Pointer ret;
				_MSTD_TRY
					// Need allocate new space
					// Rearrange space
					newBase = _reallocate(_getGrownCapacity(count));
					// Copy elements before pos
					newBase._end = _moveRange(
						(_vBase._beg),
						(pos),
						newBase._end
					);
					ret = newBase._end;
					// Copy {count} values
					uninitializedFill_N(newBase._end, count, val);
					newBase._end += count;
					// Copy rest
					newBase._end = _moveRange(
						(pos),
						(_vBase._end),
						newBase._end
					);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH

				// destroy old elements
				_destroy();
				_vBase.swap(newBase);
				return ret;
			}
		}

		template<typename InputIt>
		Pointer _aux_insertRange(Pointer pos, InputIt first, InputIt last)
		{
			SizeType range = last - first;
			if (_hasEnoughCapacity(range)) {
				_copyBackward(pos, _vBase._end, range);
				uninitializedCopy(first, last, pos);
				return pos;
			}
			else {
				_VecBase<Pointer> newBase;
				Pointer ret;
				_MSTD_TRY
					newBase = _reallocate(_getGrownCapacity(range));
					newBase._end = _moveRange(_vBase._beg, pos, newBase._end);
					ret = newBase._end;
					newBase._end = uninitializedCopy(first, last, newBase._end);
					newBase._end = _moveRange(pos, _vBase._end, newBase._end);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH

				// destroy old elements
				_destroy();
				_vBase.swap(newBase);
				return ret;
			}
		}

		void _constructHelper(Pointer pos, const ValueType &val, falseType)
		{
			::new (static_cast<void*>(pos)) ValueType(val);
		}

		void _constructHelper(Pointer pos, const ValueType &val, trueType)
		{
			::new (static_cast<void*>(pos)) ValueType(move(val));
		}

		// insert {val} before {pos}
		Pointer _aux_insert(Pointer pos, const ValueType &val)
		{
			if (_hasEnoughCapacity(1)) {
				ValueType valCopy = val;
				_copyBackward(pos, _vBase._end, 1);
				_constructHelper(pos, valCopy, falseType());

				return pos;
			}
			else {
				_VecBase<Pointer> newBase;
				Pointer ret;
				_MSTD_TRY
					newBase = _reallocate(_getGrownCapacity(1));
					newBase._end = _moveRange(_vBase._beg, pos, newBase._end);

					ret = newBase._end;
					_constructHelper(newBase._end, val, falseType());
					newBase._end += 1;

					newBase._end = _moveRange(pos, _vBase._end, newBase._end);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH

				// destroy old elements
				_destroy();
				_vBase.swap(newBase);

				return ret;
			}
		}

		// insert {val} before {pos}
		Pointer _aux_insert(Pointer pos, ValueType &&val)
		{
			if (_hasEnoughCapacity(1)) {
				_copyBackward(pos, _vBase._end, 1);
				_constructHelper(
					pos, val,
					typename conditional<isNothrowMoveConstructible<ValueType>::value,
										trueType, falseType>::type()
				);

				return pos;
			}
			else {
				_VecBase<Pointer> newBase;
				Pointer ret;
				_MSTD_TRY
					newBase = _reallocate(_getGrownCapacity(1));
					newBase._end = _moveRange(_vBase._beg, pos, newBase._end);

					ret = newBase._end;
					_constructHelper(
						newBase._end, val,
						typename conditional<isNothrowMoveConstructible<ValueType>::value,
											trueType, falseType>::type()
					);
					newBase._end += 1;

					newBase._end = _moveRange(pos, _vBase._end, newBase._end);
				_MSTD_CATCH_ALL
					_cleanUp(newBase);
					throw;
				_MSTD_END_CATCH

				// destroy old elements
				_destroy();
				_vBase.swap(newBase);

				return ret;
			}
		}

		void _destroy()
		{
			_cleanUp(_vBase);
			_vBase._beg = nullptr;
			_vBase._end = nullptr;
			_vBase._capacity = nullptr;
		}

		void _aux_cleanUp(const _VecBase<Pointer> &vBase, falseType)
		{
			for (auto it = vBase._beg; it != vBase._end; ++it) {
				it->~ValueType();
			}
			_alloc.deallocate(vBase._beg, vBase._capacity - vBase._beg);
		}

		void _aux_cleanUp(const _VecBase<Pointer> &vBase, trueType)
		{
			_alloc.deallocate(vBase._beg, vBase._capacity - vBase._beg);
		}

		void _cleanUp(const _VecBase<Pointer> &vBase)
		{
			_aux_cleanUp(
				vBase,
				typename conditional<
					isTriviallyDestructible<ValueType>::value,
					trueType, falseType
				>::type()
			);
		}

		_VecBase<Pointer> _reallocate(SizeType reqSize)
		{			
			auto newBeg = static_cast<Pointer>(_alloc.allocate(reqSize));
			auto newEnd = newBeg;
			auto newCap = newBeg + reqSize;

			return _VecBase<Pointer>(newBeg, newEnd, newCap);
		}

		bool _hasEnoughCapacity(SizeType reqSize)
		{
			if (size() + reqSize > capacity()) {
				return false;
			}
			else {
				return true;
			}
		}

		SizeType _getGrownCapacity(SizeType reqSize)
		{
			if (capacity() == 0) {
				return reqSize;
			}
			else {
				SizeType minSize = reqSize + size();
				if (minSize <= capacity() * 2) {
					return  capacity() * 2;
				}
				else {
					return minSize;
				}
			}
		}

		bool _verifyRange(SizeType pos) const
		{
			return pos < static_cast<SizeType>(_vBase._end - _vBase._beg);
		}

		void _aux_resize(SizeType count, const ValueType &val = ValueType())
		{
			if (count > capacity()) {
				auto fillCount = count - size();
				reserve(count);
				_aux_insertRange(_vBase._end, fillCount, val);
			}
			else if (count > size()) {
				auto fillCount = count - size();
				_aux_insertRange(_vBase._end, fillCount, val);
			}
			else if (count == size()) {
				// no-op
			}
			else {
				auto delStart = _vBase._beg + count;
				for (auto it = delStart; it < _vBase._end; ++it) {
					_alloc.destroy(it);
				}
				_vBase._end = delStart;
			}
		}
	};

	// Compare operations of Vector
	template<typename T, typename Alloc>
	bool operator==(const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		if (lhs.size() == rhs.size()) {
			for (size_t i = 0; i < lhs.size(); ++i) {
				if (!(lhs[i] == rhs[i])) {
					return false;
				}
			}
			return true;
		}

		return false;
	}

	template<typename T, typename Alloc>
	bool operator!=(const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, typename Alloc>
	bool operator<( const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		size_t i = 0;
		size_t j = 0;
		while (i < lhs.size() && j < rhs.size()) {
			if (lhs[i] < rhs[j]) {
				return true;
			}
			else if (rhs[j] < lhs[i]) {
				return false;
			}
		}
		
		return i == lhs.size() && j != rhs.size();
	}

	template<typename T, typename Alloc>
	bool operator<=(const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		size_t i = 0;
		size_t j = 0;
		while (i < lhs.size() && j < rhs.size()) {
			if (lhs[i] < rhs[j]) {
				return true;
			}
			else if (rhs[j++] < lhs[i++]) {
				return false;
			}
		}

		return i == lhs.size();
	}

	template<typename T, typename Alloc>
	bool operator>(const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename T, typename Alloc>
	bool operator>=(const Vector<T, Alloc> &lhs,
					const Vector<T, Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T, typename Alloc>
	void swap(Vector<T, Alloc> &lhs, Vector<T, Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}
}