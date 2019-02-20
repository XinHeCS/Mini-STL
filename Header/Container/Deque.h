#pragma once

// Deque standard header

// Mini-STL header
#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>

// STL header and cpp standard header
#include <initializer_list>
#include <stdexcept>

namespace MSTD {

// Default buffer size (elements per buffer)
#define DEQUE_BUFFER_SIZE (sizeof(ValueType) >= 512 ? 1 : 512 / sizeof(ValueType))
// Default map list size
#define DEQUE_MAP_SIZE 8

	template<typename _Deque>
	class _DequeConstIterator
	{
	protected:
		using _SizeType = typename _Deque::SizeType;
		using _BufferPtr = typename _Deque::_BufferPtr;
		using _MapPtr = typename _Deque::_MapPtr;
	public:
		using IteratorCategory = MSTD::RandomAccessIteratorTag;
		using DifferenceType = typename _Deque::DifferenceType;
		using ValueType = typename _Deque::ValueType;
		using Pointer = typename _Deque::ConstPointer;
		using Reference = typename _Deque::ConstReference;

		_DequeConstIterator() :
			_first(nullptr),
			_last(nullptr),
			_cur(nullptr),
			_node(nullptr)
		{}

		_DequeConstIterator(_MapPtr node) :
			_first(*node),
			_last(*node + DEQUE_BUFFER_SIZE),
			_cur(_first),
			_node(node)
		{}

		_DequeConstIterator(_BufferPtr pos, _MapPtr node) :
			_first(*node),
			_last(*node + DEQUE_BUFFER_SIZE),
			_cur(pos),
			_node(node)
		{}

		void _setNode(_MapPtr node)
		{
			_first = *node;
			_last = *node + DEQUE_BUFFER_SIZE;
			_cur = _first;
			_node = node;
		}

		Reference operator*() const
		{
			return *_cur;
		}

		Pointer operator->() const
		{
			return MSTD::addressof(this->operator*());
		}

		_DequeConstIterator& operator++()
		{
			if (_cur == _last - 1) {
				_setNode(_node + 1);
				return *this;
			}
			++_cur;
			return *this;
		}

		_DequeConstIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return (tmp);
		}

		_DequeConstIterator& operator--()
		{
			if (_cur == _first) {
				_setNode(_node - 1);
				_cur = _last;				
			}
			--_cur;
			return *this;
		}

		_DequeConstIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_DequeConstIterator& operator+=(DifferenceType diff)
		{
			// Absolute offset from first
			DifferenceType offset = diff + (_cur - _first);

			DifferenceType buffSkip = offset >= 0 ?
				offset / DEQUE_BUFFER_SIZE
				: -((offset + 1) / static_cast<DifferenceType>(DEQUE_BUFFER_SIZE) + 1);
			_setNode(_node + buffSkip);
			_cur = _first + (offset - buffSkip * DEQUE_BUFFER_SIZE);

			return *this;
		}

		_DequeConstIterator operator+(DifferenceType diff) const
		{
			auto tmp = *this;
			return (tmp += diff);
		}

		friend _DequeConstIterator operator+(DifferenceType diff, const _DequeConstIterator &it)
		{
			return it + diff;
		}

		_DequeConstIterator& operator-=(DifferenceType diff)
		{
			return *this += -diff;
		}

		_DequeConstIterator operator-(DifferenceType diff) const
		{
			return *this + -diff;
		}

		DifferenceType operator-(const _DequeConstIterator &that) const
		{
			return (_node - that._node - 1) * DEQUE_BUFFER_SIZE +
				(_cur - _first) + (that._last - that._cur);
		}

		Reference operator[](_SizeType index) const
		{
			return *(*this + index);
		}

		bool operator==(const _DequeConstIterator &that) const
		{
			return _cur == that._cur;
		}

		bool operator!=(const _DequeConstIterator &that) const
		{
			return !(_cur == that._cur);
		}

		bool operator<(const _DequeConstIterator &that) const
		{
			return _node < that._node ? true
				: _node != that._node ? false
				: _cur < that._cur;
		}

		bool operator<=(const _DequeConstIterator &that) const
		{
			return _node < that._node ? true
				: _node != that._node ? false
				: _cur <= that._cur;
		}

		bool operator>(const _DequeConstIterator &that) const
		{
			return !(*this <= that);
		}

		bool operator>=(const _DequeConstIterator &that) const
		{
			return !(*this < that);
		}

		// data member
		_BufferPtr _first; // Pointer to begin position of current buffer
		_BufferPtr _last; // Pointer to past to end position of current buffer
		_BufferPtr _cur; // Pointer to object this iterator referring to
		_MapPtr _node; // Pointer to current buffer pointer
	};

	template<typename _Deque>
	class _DequeIterator : public _DequeConstIterator<_Deque>
	{
	protected:
		using _SizeType = typename _Deque::SizeType;
		using _BufferPtr = typename _Deque::_BufferPtr;
		using _MapPtr = typename _Deque::_MapPtr;
	public:
		using IteratorCategory = MSTD::RandomAccessIteratorTag;
		using DifferenceType = typename _Deque::DifferenceType;
		using ValueType = typename _Deque::ValueType;
		using Pointer = typename _Deque::Pointer;
		using Reference = typename _Deque::Reference;

		_DequeIterator() :
			_DequeConstIterator()
		{}

		// inherit other ctors from const iterator
		using _DequeConstIterator<_Deque>::_DequeConstIterator;

		Reference operator*() const
		{
			return *_cur;
		}

		Pointer operator->() const
		{
			return MSTD::addressof(this->operator*());
		}


		_DequeIterator& operator++()
		{
			if (_cur == _last - 1) {
				_setNode(_node + 1);
				return *this;
			}
			++_cur;
			return *this;
		}

		_DequeIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return (tmp);
		}

		_DequeIterator& operator--()
		{
			if (_cur == _first) {
				_setNode(_node - 1);
				_cur = _last;				
			}
			--_cur;
			return *this;
		}

		_DequeIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_DequeIterator& operator+=(DifferenceType diff)
		{
			// Absolute offset from first
			DifferenceType offset = diff + (_cur - _first);

			DifferenceType buffSkip = offset >= 0 ?
				offset / DEQUE_BUFFER_SIZE 
				: -((offset + 1) / static_cast<DifferenceType>(DEQUE_BUFFER_SIZE) + 1);
			_setNode(_node + buffSkip);
			_cur = _first + (offset - buffSkip * DEQUE_BUFFER_SIZE);

			return *this;
		}

		_DequeIterator operator+(DifferenceType diff) const
		{
			auto tmp = *this;
			return (tmp += diff);
		}

		_DequeIterator& operator-=(DifferenceType diff)
		{
			return *this += -diff;
		}

		_DequeIterator operator-(DifferenceType diff) const
		{
			return *this + -diff;
		}

		DifferenceType operator-(const _DequeIterator &that) const
		{
			return (_node - that._node - 1) * DEQUE_BUFFER_SIZE +
				(_cur - _first) + (that._last - that._cur);
		}

		Reference operator[](_SizeType index) const
		{
			return *(*this + index);
		}
	};

	template<
		typename T,
		typename Alloc = MSTD::Allocator<T>
	> class Deque
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
		using Iterator = _DequeIterator<Deque>;
		using ConstIterator = _DequeConstIterator<Deque>;
		using ConstReverseIterator = ReverseIterator<ConstIterator>;
		using ReverseIterator = ReverseIterator<Iterator>;

		using _BufferPtr = Pointer;
		using _MapPtr = _BufferPtr * ;
		using _BufferAlloc = MSTD::Allocator<ValueType>;
		using _MapAlloc = MSTD::Allocator<_BufferPtr>;

		static_assert(isSame<ValueType, typename AllocatorTraits<Alloc>::ValueType>::value,
			"Allocator require the same type T with Deque<T>");

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		/////////////////////////////////////

		Deque() :
			_map(nullptr),			
			_beg(),
			_end(),
			_alloc(),
			_bufferAlloc(),
			_mapAlloc()
		{
			_initialize();
		}

		Deque(const Alloc &alloc) :
			_map(nullptr),
			_beg(),
			_end(),
			_alloc(alloc),
			_bufferAlloc(),
			_mapAlloc()
		{
			_initialize();
		}

		Deque(SizeType count, const ValueType &val, const Alloc &alloc = Alloc()) :
			Deque(alloc)
		{
			while (count--) {
				_auxInsert(_end, val);
			}
		}

		explicit Deque(SizeType count) :
			Deque(count, ValueType(), Alloc())
		{}

		template<
			typename InputIt,
			typename = typename enableIf<
				!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value,
				void
			>::type
		>
		Deque(InputIt first, InputIt last, const Alloc &alloc) :
			Deque(alloc)
		{
			for (; first != last; ++first) {
				_auxInsert(_end, *first);
			}
		}

		Deque(const Deque &other, const Alloc &alloc = Alloc()) :
			Deque(other.begin(), other.end(), alloc)
		{}

		Deque(Deque &&that, const Alloc &alloc) noexcept:
			_map(move(that._map)),
			_mapSize(move(that._mapSize)),
			_beg(move(that._beg)),
			_end(move(that.end)),
			_alloc(alloc),
			_bufferAlloc(),
			_mapAlloc()
		{
			that._map = nullptr;
			that._mapSize = 0;
			that._beg._first = that._beg._last = that._beg._cur = nullptr;
			that._end._first = that._end._last = that._end._cur = nullptr;
		}

		Deque(Deque &&that) noexcept :
			Deque(move(that), that._alloc)
		{}

		Deque(std::initializer_list<ValueType> il, const Alloc &alloc = Alloc()) :
			Deque(il.begin(), il.end(), alloc)
		{}

		~Deque()
		{
			_destroy();
		}

		Deque& operator=(const Deque& that)
		{
			if (this != addressof(that)) {
				_destroy();
				_alloc = that._alloc;
				_initialize();
				for (auto it = that.begin(); it != that.end(); ++it) {
					_auxInsert(_end, *it);
				}
			}
			return *this;
		}

		Deque& operator=(Deque &&that) noexcept
		{
			if (this != addressof(that)) {
				_destroy();
				_alloc = that._alloc;
				_map = move(that._map);
				_mapSize = that._mapSize;
				_beg = move(that._beg);
				_end = move(that._end);
				that._map = nullptr;
				that._mapSize = 0;
				that._beg._first = that._beg._last = that._beg._cur = nullptr;
				that._end._first = that._end._last = that._end._cur = nullptr;
			}
			return *this;
		}

		Deque& operator=(std::initializer_list<ValueType> il)
		{
			_destroy();			
			_initialize();
			for (auto &&val : il) {
				_auxInsert(_end, val);
			}
			return *this;
		}

		Alloc getAllocator() noexcept
		{
			return _alloc;
		}

		/////////////////////////////////////
		//
		//			Element access
		//
		/////////////////////////////////////

		Reference at(SizeType pos)
		{
			_verifyRange(pos);
			return (*this)[pos];
		}

		ConstReference at(SizeType pos) const
		{
			_verifyRange(pos);
			return (*this)[pos];
		}

		Reference operator[](SizeType pos)
		{
			return *(_beg + pos);
		}

		ConstReference operator[](SizeType pos) const
		{
			return *(_beg + pos);
		}

		Reference front()
		{
			return *(_beg);
		}

		ConstReference front() const
		{
			return *_beg;
		}

		Reference back()
		{
			return *(_end - 1);
		}

		ConstReference back() const
		{
			return *(_end - 1);
		}

		/////////////////////////////////////
		//
		//			Iterators
		//
		/////////////////////////////////////

		Iterator begin() noexcept
		{
			return _beg;
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(_beg);
		}

		ConstIterator cbegin() const noexcept
		{
			return ConstIterator(_beg);
		}

		Iterator end() noexcept
		{
			return _end;
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(_end);
		}

		ConstIterator cend() const noexcept
		{
			return ConstIterator(_end);
		}

		ReverseIterator rbegin() noexcept
		{
			return _end;
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstIterator(_end);
		}

		ConstReverseIterator crbegin() const noexcept
		{
			return ConstIterator(_end);
		}

		ReverseIterator rend() noexcept
		{
			return _beg;
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstIterator(_beg);
		}

		ConstReverseIterator crend() const noexcept
		{
			return ConstIterator(_beg);
		}

		/////////////////////////////////////
		//
		//			Capacity
		//
		/////////////////////////////////////

		bool empty() const noexcept
		{
			return _beg == _end;
		}

		SizeType size() const noexcept
		{
			return _end - _beg;
		}

		SizeType maxSize() const noexcept
		{
			return _alloc.maxSize();
		}

		/////////////////////////////////////
		//
		//			Modifiers
		//
		/////////////////////////////////////

		void clear() noexcept
		{
			_destroy();
			_initialize();
		}

		void pushBack(const ValueType &val)
		{
			_auxInsert(end(), val);
		}

		void pushBack(ValueType &&val)
		{
			_auxInsert(end(), MSTD::move(val));
		}

		void pushFront(const ValueType &val)
		{
			_auxInsert(begin(), val);
		}

		void pushFront(ValueType &&val)
		{
			_auxInsert(begin(), MSTD::move(val));
		}

		void popBack()
		{
			_auxErase(_end - 1);
		}

		void popFront()
		{
			_auxErase(_beg);
		}

		Iterator erase(ConstIterator pos)
		{
			return _auxErase(pos);
		}

		Iterator erase(ConstIterator first, ConstIterator last)
		{
			while (first != last) {
				first = _auxErase(first);
			}
			return Iterator(last._cur, last._node);
		}

		template<typename... Args>
		Iterator emplace(ConstIterator pos, Args&&... args)
		{
			return _auxEmplace(pos, forward<Args>(args)...);
		}

		template<typename... Args>
		void emplaceBack(Args&&... args)
		{
			_auxEmplace(_end, forward<Args>(args)...);
		}

		template<typename... Args>
		void emplaceFront(Args&&... args)
		{
			_auxEmplace(_beg, forward<Args>(args)...);
		}

		Iterator insert(ConstIterator pos, const ValueType &val)
		{
			return _auxInsert(pos, val);
		}

		Iterator insert(ConstIterator pos, ValueType &&val)
		{
			return _auxInsert(pos, move(val));
		}

		Iterator insert(ConstIterator pos, SizeType count, const ValueType &val)
		{
			Iterator ret(pos._cur, pos._node);
			while (count--) {
				ret = _auxInsert(ret, val);
			}
			return ret;
		}

		template<
			typename InputIt,
			typename = typename enableIf<
			!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value,
			void
			>::type
		>
		Iterator insert(ConstIterator pos, InputIt first, InputIt last)
		{
			Iterator ret(pos._cur, pos._node);
			for (; first != last; ++first) {
				ret = _auxInsert(ret, *first);
			}
			return ret;
		}

		Iterator insert(ConstIterator pos, std::initializer_list<ValueType> il)
		{
			return insert(pos, il.begin(), il.end());
		}

		void swap(Deque &that) noexcept
		{
			using std::swap;
			swap(_map, that._map);
			swap(_mapSize, that._mapSize);
			swap(_beg, that._beg);
			swap(_end, that._end);
			swap(_alloc, that._alloc);
		}
		
		
	protected:
		// data member
		_MapPtr _map; // Pointer first node of buffer array
		SizeType _mapSize; // Size of current map array
		Iterator _beg; // Pointer to first buffer
		Iterator _end; // Pointer to last buffer
		// Allocators
		Alloc _alloc;
		_BufferAlloc _bufferAlloc;
		_MapAlloc _mapAlloc;

	private:

		void _destroy()
		{
			_cleanUp();
			for (SizeType i = 0; i < _mapSize; ++i) {
				if (*(_map + i)) {
					_bufferAlloc.deallocate(*(_map + i), DEQUE_BUFFER_SIZE);
					*(_map + i) = nullptr;
				}
			}
			_mapAlloc.deallocate(_map, _mapSize);
			_map = nullptr;
			_mapSize = 0;
		}

		void _auxCleanUp(trueType)
		{
			// no-op
		}

		void _auxCleanUp(falseType)
		{
			for (auto it = begin(); it != end(); ++it) {
				_alloc.destroy(it._cur);
			}
		}

		// Free the elements in range [begin, end)
		void _cleanUp()
		{
			_auxCleanUp(
				typename conditional<
					isTriviallyDestructible<ValueType>::value,
					trueType, falseType
				>::type()
			);
		}

		void _verifyRange(SizeType pos) const
		{
			if (pos < 0 || pos >= size()) {
				throw std::out_of_range("Access position out of Deque<T>");
			}
		}

		_MapPtr _getMap(SizeType n)
		{
			auto map = _mapAlloc.allocate(n);
			for (auto cur = 0; cur < n; ++cur) {
				map[cur] = nullptr;
			}
			return map;
		}

		void _initialize()
		{			
			_mapSize = DEQUE_MAP_SIZE;
			_map = _getMap(DEQUE_MAP_SIZE);
			_map[DEQUE_MAP_SIZE / 2] = _bufferAlloc.allocate(DEQUE_BUFFER_SIZE);
			// Set {_beg} points to the middle of map
			// and hence Deque can grow up towards two directions
			_beg._setNode(_map + DEQUE_MAP_SIZE / 2);
			_end._setNode(_beg._node);
		}

		// Add another {addCnt} nodes
		void _reallocateMap(SizeType addCnt, bool addAtFront)
		{
			SizeType oldNodeNum = _end._node - _beg._node + 1;
			SizeType newCnt = oldNodeNum + addCnt;
			_MapPtr nStart;

			if (_mapSize > 2 * newCnt) {
				// Enough space, just move content
				nStart = _map + (_mapSize - newCnt) / 2
					+ (addAtFront ? addCnt : 0);
				std::memmove(nStart, _beg._node, oldNodeNum * sizeof(_MapPtr));
			}
			else {
				// Reallocate map
				SizeType newMapSize = _mapSize + (_mapSize > newCnt ? _mapSize : newCnt) + 2;
				auto newMapPtr = _getMap(newMapSize);
				nStart = newMapPtr + (newMapSize - newCnt) / 2
					+ (addAtFront ? addCnt : 0);
				// Copy nodes pointers from old map
				std::memmove(nStart, _beg._node, oldNodeNum * sizeof(_MapPtr));
				// Deallocate old map
				_mapAlloc.deallocate(_map, _mapSize);
				// Set new map
				_map = newMapPtr;
				_mapSize = newMapSize;
			}

			// Set new start mode
			_beg._node = nStart;
			_end._node = nStart + oldNodeNum - 1;
		}

		// Copy range [first, last) forward starting at {dest}
		void _copyForward(Iterator first, Iterator last, Iterator dest)
		{
			for (; first != last; ++first, ++dest) {
				*dest = *first;
			}
		}

		// Copy range [first, last) backward to range [*, dest)
		void _copyBackward(Iterator first, Iterator last, Iterator dest)
		{
			if (first != last) {
				while (last != first) {
					*--dest = *--last;
				}
				*dest = *last;
			}
		}

		// Check whether there exists enough buffer and map node 
		// to insert an object.
		// If not, allocate more space to fit the requirement
		void _checkCapacity(bool checkFront)
		{
			if (checkFront) {
				// Check front
				// Check buffer space
				if (_beg._cur == _beg._first) {
					// No more space in current buffer
					// Check previous node
					if (_beg._node == _map) {
						// Reallocate map
						_reallocateMap(1, true);
					}
					// Check previous buffer
					auto preBuffer = _beg._node - 1;
					if (!*preBuffer) {
						// Allocate new buffer
						*preBuffer = _bufferAlloc.allocate(DEQUE_BUFFER_SIZE);
					}
				}
			}
			else {
				// Check back
				// Check buffer space
				if (_end._cur == _end._last - 1) {
					if (_end._node == _map + _mapSize - 1) {
						_reallocateMap(1, false);
					}
					// Check next buffer
					auto nextBuffer = _end._node + 1;
					if (!*nextBuffer) {
						// Allocate new buffer
						*nextBuffer = _bufferAlloc.allocate(DEQUE_BUFFER_SIZE);
					}
				}
			}
		}

		Iterator _auxInsert(ConstIterator pos, const ValueType &val)
		{
			// Check the dsitance between (begin(), pos) and (pos, end()) 
			// and choose to move less elements
			DifferenceType rawPos = pos - _beg;
			const ValueType vCopy = val;
			if (rawPos < static_cast<DifferenceType>(size() / 2)) {
				// Move first half elements
				_checkCapacity(true);
				_copyForward(_beg, _beg + rawPos, _beg - 1);
				// Insert new value
				auto newPos = _beg + rawPos - 1;
				_alloc.construct(newPos._cur, vCopy);
				// Upadte begin iterator
				--_beg;
				return newPos;
			}
			else {
				// Move last half elements
				_checkCapacity(false);
				auto nEnd = _end;
				_copyBackward(_beg + rawPos, _end, _end + 1);
				// Insert new value
				_alloc.construct((_beg + rawPos)._cur, vCopy);
				// Update end iterator
				++_end;
				return _beg + rawPos;
			}
		}

		Iterator _auxInsert(ConstIterator pos, ValueType &&val)
		{
			// Check the dsitance between (begin(), pos) and (pos, end()) 
			// and choose to move less elements			
			DifferenceType rawPos = pos - _beg;
			if (rawPos < static_cast<DifferenceType>(size() / 2)) {
				// Move first half elements
				_checkCapacity(true);
				_copyForward(_beg, _beg + rawPos, _beg - 1);
				// Insert new value
				auto newPos = _beg + rawPos - 1;
				_alloc.construct(newPos._cur, MSTD::move(val));
				// Upadte begin iterator
				--_beg;
				return newPos;
			}
			else {
				// Move last half elements
				_checkCapacity(false);
				auto nEnd = _end;
				_copyBackward(_beg + rawPos, _end, _end + 1);
				// Insert new value
				_alloc.construct((_beg + rawPos)._cur, MSTD::move(val));
				// Update end iterator
				++_end;
				return _beg + rawPos;
			}
		}

		template<typename... Args>
		Iterator _auxEmplace(ConstIterator pos, Args&&... args)
		{
			// Check the dsitance between (begin(), pos) and (pos, end()) 
			// and choose to move less elements			
			DifferenceType rawPos = pos - _beg;
			if (rawPos < static_cast<DifferenceType>(size() / 2)) {
				// Move first half elements
				_checkCapacity(true);
				_copyForward(_beg, _beg + rawPos, _beg - 1);
				// Insert new value
				auto newPos = _beg + rawPos - 1;
				_alloc.construct(newPos._cur, forward<Args>(args)...);
				// Upadte begin iterator
				--_beg;
				return newPos;
			}
			else {
				// Move last half elements
				_checkCapacity(false);
				auto nEnd = _end;
				_copyBackward(_beg + rawPos, _end, _end + 1);
				// Insert new value
				_alloc.construct((_beg + rawPos)._cur, forward<Args>(args)...);
				// Update end iterator
				++_end;
				return _beg + rawPos;
			}
		}

		void _shrinkCapacity(bool shrinkFront)
		{
			if (shrinkFront) {
				auto preNode = _beg._node - 1;
				if (*preNode) {
					_bufferAlloc.deallocate(*preNode, DEQUE_BUFFER_SIZE);
					*preNode = nullptr;
				}
			}
			else {
				auto postNode = _end._node + 1;
				if (*postNode) {
					_bufferAlloc.deallocate(*postNode, DEQUE_BUFFER_SIZE);
					*postNode = nullptr;
				}
			}
		}

		Iterator _auxErase(ConstIterator pos)
		{
			DifferenceType rawPos = pos - _beg;
			_alloc.destroy(pos._cur);
			if (rawPos < static_cast<DifferenceType>(size() / 2)) {
				_copyBackward(_beg, _beg + rawPos, _beg + rawPos + 1);
				++_beg;
				_shrinkCapacity(true);
			}
			else {
				_copyForward(_beg + rawPos + 1, _end, _beg + rawPos);
				--_end;
				_shrinkCapacity(false);
			}
			return _beg + rawPos;
		}
	};

	template<typename T, typename Alloc>
	bool operator==(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
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

	template<typename T, typename Alloc>
	bool operator!=(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, typename Alloc>
	bool operator<(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
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

	template<typename T, typename Alloc>
	bool operator<=(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
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

	template<typename T, typename Alloc>
	bool operator>(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename T, typename Alloc>
	bool operator>=(const Deque<T, Alloc> &lhs, const Deque<T, Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T, typename Alloc>
	void swap(Deque<T, alloc> &lhs, Deque<T, Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}

}