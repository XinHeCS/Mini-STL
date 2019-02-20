#pragma once

// List standard header

// Mini-STL header
#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <TypeInfo/TypeTraits.h>
#include <Iterator/Iterator.h>

// STL header and cpp standard header
#include <initializer_list>
#include <functional>
#include <stdexcept>

namespace MSTD {

	// List node
	template<typename T>
	class _ListNode
	{
	public:
		using _NodePtr = _ListNode * ;

		// Constructors
		_ListNode() :
			_pre(nullptr),
			_next(nullptr)
		{}

		_ListNode(const T &val) :
			_pre(nullptr),
			_next(nullptr),
			_val(val)
		{}

		_ListNode(T &&val) :
			_pre(nullptr),
			_next(nullptr),
			_val(move(val))
		{}

		_ListNode(_NodePtr pre, _NodePtr next) :
			_pre(pre),
			_next(next)
		{}

		_ListNode(_NodePtr pre, _NodePtr next, const T &val) :
			_pre(pre),
			_next(next),
			_val(val)
		{}

		_ListNode(_NodePtr pre, _NodePtr next, T &&val) :
			_pre(pre),
			_next(next),
			_val(move(val))
		{}

		_NodePtr _next; // Successor node
		_NodePtr _pre; // Predecessor node
		T _val; // element
	};

	// Const iterator
	template<typename _List>
	class _ListConstIterator
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;

		using _NodePtr = typename _List::_NodePtr;

		using DifferenceType = typename _List::DifferenceType;
		using ValueType = typename _List::ValueType;
		using Pointer = typename _List::ConstPointer;
		using Reference = typename _List::ConstReference;

		_ListConstIterator() :
			_ptr(nullptr)
		{}

		_ListConstIterator(_NodePtr ptr) :
			_ptr(ptr)
		{}

		// Return stored value in current node
		Reference operator*() const
		{
			return _ptr->_val;
		}

		// Return Pointer to element object
		Pointer operator->() const
		{
			return addressof(this->operator*());
		}

		_ListConstIterator& operator++()
		{
			_ptr = _ptr->_next;
			return *this;
		}

		_ListConstIterator operator++(int)
		{
			auto tmp = _ptr;
			_ptr = _ptr->_next;

			return _ListConstIterator(tmp);
		}

		_ListConstIterator& operator--()
		{
			_ptr = _ptr->_pre;
			return *this;
		}

		_ListConstIterator operator--(int)
		{
			auto tmp = _ptr;
			_ptr = _ptr->_pre;

			return _ListConstIterator(tmp);
		}

		bool operator==(const _ListConstIterator &that) const
		{
			return _ptr == that._ptr;
		}

		bool operator!=(const _ListConstIterator &that) const
		{
			return !(_ptr == that._ptr);
		}

		_NodePtr _ptr;
	};

	// Iterator
	template<typename _List>
	class _ListIterator : public _ListConstIterator<_List>
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;

		using _NodePtr = typename _List::_NodePtr;

		using DifferenceType = typename _List::DifferenceType;
		using ValueType = typename _List::ValueType;
		using Pointer = typename _List::Pointer;
		using Reference = typename _List::Reference;

		_ListIterator() :
			_ListConstIterator()
		{}

		_ListIterator(_NodePtr ptr) :
			_ListConstIterator(ptr)
		{}

		// Return stored value in current node
		Reference operator*() const
		{
			return _ptr->_val;
		}

		// Return Pointer to element object
		Pointer operator->() const
		{
			return addressof(this->operator*());
		}

		_ListIterator& operator++()
		{
			_ptr = _ptr->_next;
			return *this;
		}

		_ListIterator operator++(int)
		{

			auto tmp = _ptr;
			_ptr = _ptr->_next;

			return _ListIterator(tmp);
		}

		_ListIterator& operator--()
		{
			_ptr = _ptr->_pre;
			return *this;
		}

		_ListIterator operator--(int)
		{
			auto tmp = _ptr;
			_ptr = _ptr->_pre;

			return _ListIterator(tmp);
		}
	};

	template<
		typename T,
		typename Alloc = MSTD::Allocator<T>
	> class List
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
		using Iterator = _ListIterator<List>;
		using ConstIterator = _ListConstIterator<List>;
		using ConstReverseIterator = ReverseIterator<ConstIterator>;
		using ReverseIterator = ReverseIterator<Iterator>;

		using _NodePtr = typename _ListNode<ValueType>::_NodePtr;
		//using _NodeAlloc = typename MSTD::AllocatorTraits<Alloc>::rebind<_NodePtr>;
		using _NodeAlloc = MSTD::Allocator<_ListNode<ValueType>>;

		static_assert(isSame<ValueType, typename AllocatorTraits<Alloc>::ValueType>::value,
			"Allocator require the same type T with List<T>");

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		/////////////////////////////////////		

		List() :
			_pHead(nullptr),
			_alloc(),
			_nodeAlloc(),
			_size(0)
		{
			_initialize();
		}

		explicit List(const Alloc &alloc) :
			_pHead(nullptr),
			_alloc(alloc),
			_nodeAlloc(),
			_size(0)
		{
			_initialize();
		}

		List(SizeType count, const ValueType &val, const Alloc &alloc = Alloc()) :
			List(alloc)
		{			
			for (; 0 < count; --count) {
				_auxInsert(begin()._ptr, val);
			}
		}

		explicit List(SizeType count, const Alloc &alloc = Alloc()) :
			List(alloc)
		{			
			for (; 0 < count; --count) {
				_auxInsert(begin()._ptr, ValueType());
			}
		}

		template<
			typename InputIt,
			typename = typename enableIf<
				!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value
			>::type
		>
		List(InputIt first, InputIt last, const Alloc &alloc = Alloc()) :
			List(alloc)
		{			
			_auxInsertRange(end()._ptr, first, last);
		}

		List(const List &that) :
			List()
		{			
			_auxInsertRange(end()._ptr, that.begin(), that.end());
		}

		List(const List &that, const Alloc &alloc) :
			List(alloc)
		{			
			_auxInsertRange(end()._ptr, that.begin(), that.end());
		}

		List(List &&that) noexcept:
			_pHead(move(that._pHead)),
			_alloc(move(that._alloc)),
			_nodeAlloc(move(that._nodeAlloc)),
			_size(move(that._size))
		{
			that._pHead = nullptr;
			that._size = 0;
		}

		List(List &&that, const Alloc &alloc) :
			_pHead(move(that._pHead)),
			_alloc(alloc),
			_nodeAlloc(move(that._nodeAlloc)),
			_size(move(that._size))
		{
			that._pHead = nullptr;
			that._size = 0;
		}

		List(std::initializer_list<ValueType> li, const Alloc &alloc = Alloc()) :
			List(alloc)
		{			
			_auxInsertRange(end()._ptr, li.begin(), li.end());
		}

		~List()
		{
			_destroy();
		}

		List& operator=(const List &that)
		{
			if (this != MSTD::addressof(that)) {
				assign(that.begin(), that.end());
			}

			return *this;
		}

		List& operator=(List &&that) noexcept
		{
			if (this != MSTD::addressof(that)) {
				_size = that._size;
				that._size = 0;
				_pHead = that._pHead;
				that._pHead = nullptr;
				_alloc = that._alloc;
				_nodeAlloc = that._nodeAlloc;
			}

			return *this;
		}

		List& operator=(std::initializer_list<ValueType> li)
		{
			assign(li.begin(), li.end());

			return *this;
		}

		void assign(SizeType count, const ValueType &val)
		{
			// Reuse old nodes
			auto valCopy = val;
			Iterator old = begin();
			for (; count > 0 && old != end(); --count) {
				_destroyNode(old._ptr);
				_constructNode(old._ptr, valCopy);
				++old;
			}
			for (; count > 0; --count) {
				_auxInsert(end()._ptr, valCopy);
			}
			// Erase the redundant elements of {this}
			_auxEraseRange(old._ptr, end()._ptr);
		}

		template<
			typename InputIt,
			typename = typename enableIf<
				!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value
			>::type
		>
		void assign(InputIt first, InputIt last)
		{
			// Reuse the ald nodes
			Iterator old = begin();
			for (; first != last && old != end(); ++first, ++old) {
				_destroyNode(old._ptr);
				_constructNode(old._ptr, *first);
			}
			// Insert the rest element of {that} into {this}, if any
			for (; first != last; ++first) {
				_auxInsert(end()._ptr, *first);
			}
			// Erase the redundant elements of {this}
			_auxEraseRange(old._ptr, end()._ptr);
		}

		Alloc getAllocator() const
		{
			return _alloc;
		}

		/////////////////////////////////////
		//
		//			Element access
		//
		/////////////////////////////////////

		ConstReference front() const
		{
			return *begin();
		}

		ConstReference back() const
		{
			return *--end();
		}
		
		/////////////////////////////////////
		//
		//			Iterators
		//
		/////////////////////////////////////

		Iterator begin() noexcept
		{
			return Iterator(_pHead->_next);
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(_pHead->_next);
		}

		ConstIterator cbegin() const noexcept
		{
			return ConstIterator(_pHead->_next);
		}

		Iterator end() noexcept
		{
			return Iterator(_pHead);
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(_pHead);
		}

		ConstIterator cend() const noexcept
		{
			return ConstIterator(_pHead);
		}

		ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(_pHead);
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(_pHead);
		}

		ConstReverseIterator crbegin() const noexcept
		{
			return ConstReverseIterator(_pHead);
		}

		ReverseIterator rend() noexcept
		{
			return ReverseIterator(_pHead->_next);
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(_pHead->_next);
		}

		ConstReverseIterator crend() const noexcept
		{
			return ConstReverseIterator(_pHead->_next);
		}

		/////////////////////////////////////
		//
		//			Capacity
		//
		/////////////////////////////////////

		bool empty() const noexcept
		{
			return _size == 0;
		}

		SizeType size() const noexcept
		{
			return _size;
		}

		SizeType maxSize() noexcept
		{
			return AllocatorTraits<Alloc>::maxSize();
		}

		/////////////////////////////////////
		//
		//			Modifiers
		//
		/////////////////////////////////////

		void clear() noexcept
		{
			_cleanUp();
		}

		Iterator insert(ConstIterator pos, const ValueType &val)
		{
			return Iterator(_auxInsert(pos._ptr, val));
		}

		Iterator insert(ConstIterator pos, ValueType &&val)
		{
			return Iterator(_auxInsert(pos._ptr, move(val)));
		}

		Iterator insert(ConstIterator pos, SizeType count, const ValueType &val)
		{
			for (; count > 0; --count) {
				pos = _auxInsert(pos._ptr, val);
			}
			return Iterator(pos._ptr);
		}

		template<
			typename InputIt,
			typename = typename enableIf<
			!isSame<typename IteratorTraits<InputIt>::IteratorCategory, void>::value
			>::type
		>
		Iterator insert(ConstIterator pos, InputIt first, InputIt last)
		{
			return Iterator(_auxInsertRange(pos._ptr, first, last));
		}

		Iterator insert(ConstIterator pos, std::initializer_list<ValueType> il)
		{
			return Iterator(_auxInsertRange(pos._ptr, il.begin(), il.end()));
		}

		template<typename... Args>
		Iterator emplace(ConstIterator pos, Args&&... args)
		{
			auto pNode = _allocateNode();
			_constructNode(pNode, forward<Args>(args)...);

			// Insert the node
			pNode->_pre = pos._ptr->_pre;
			pNode->_next = pos._ptr;
			pos._ptr->_pre->_next = pNode;
			pos._ptr->_pre = pNode;
			++_size;

			return Iterator(pNode);
		}

		Iterator erase(ConstIterator pos)
		{
			return _auxErase(pos._ptr);
		}

		Iterator erase(ConstIterator first, ConstIterator last)
		{
			return _auxEraseRange(first._ptr, last._ptr);
		}

		void pushBack(const ValueType &val)
		{
			_auxInsert(end()._ptr, val);
		}

		void pushBack(ValueType &&val)
		{
			_auxInsert(end()._ptr, move(val));
		}

		template<typename... Args>
		Reference emplaceBack(Args&&... args)
		{
			auto node = emplace(cend(), forward<Args>(args)...);
			return node._ptr->_val;
		}

		void popBack()
		{
			_auxErase((--end())._ptr);
		}

		void pushFront(const ValueType &val)
		{
			_auxInsert(begin()._ptr, val);
		}

		void pushFront(ValueType &&val)
		{
			_auxInsert(begin()._ptr, move(val));
		}

		template<typename... Args>
		Reference emplaceFront(Args&&... args)
		{
			auto node = emplace(cbegin(), forward<Args>(args)...);
			return node._ptr->_val;
		}

		void popFront()
		{
			_auxErase(begin()._ptr);
		}

		void resize(SizeType count)
		{
			_auxResize(count);
		}

		void resize(SizeType count, const ValueType &val)
		{
			_auxResize(count, val);
		}

		void swap(List &that) noexcept
		{
			using std::swap;
			swap(_pHead, that._pHead);
			swap(_alloc, that._alloc);
			swap(_nodeAlloc, that._nodeAlloc);
			swap(_size, that._size);
		}

		/////////////////////////////////////
		//
		//			Modifiers
		//
		/////////////////////////////////////

		void merge(List &that)
		{
			merge(that, std::less<ValueType>());
		}

		template<typename Comp>
		void merge(List &that, Comp comp)
		{
			if (this != addressof(that)) {
				auto iBeg = begin();
				auto jBeg = that.begin();
				while (jBeg != that.end() && iBeg != end()) {
					if (comp(*jBeg, *iBeg)) {
						auto jNext = jBeg._ptr->_next;
						_auxSplice(iBeg._ptr, jBeg._ptr, jNext);
						jBeg._ptr = jNext;
					}
					else {
						++iBeg;
					}
				}

				_auxSplice(end()._ptr, jBeg._ptr, that.end()._ptr);
				_size += that._size;
				that._size = 0;
			}
		}

		void splice(ConstIterator pos, List &that)
		{
			if (this != addressof(that)) {
				_auxSplice(pos._ptr, that.begin()._ptr, that.end()._ptr);
				_size += that._size;
				that._size = 0;
			}
		}

		void splice(ConstIterator pos, List &that, ConstIterator it)
		{
			if ((this != addressof(that))) {
				_auxSplice(pos._ptr, it._ptr, it._ptr->_next);
				_size += 1;
				that._size -= 1;
			}
			else if ((pos != it) && (it != that.end())) {
				_auxSplice(pos._ptr, it._ptr, it._ptr->_next);
			}
		}

		void splice(ConstIterator pos, List &that, ConstIterator first, ConstIterator last)
		{
			if (this != addressof(that)) {
				auto dis = distance(first, last);
				_size += dis;
				that._size -= dis;
			}
			_auxSplice(pos._ptr, first._ptr, last._ptr);
		}

		void remove(const ValueType &val)
		{
			auto it = begin();
			while (it != end()) {
				if (*it == val) {
					it = erase(it);
				}
				else {
					++it;
				}
			}
		}

		template<typename UnaryPre>
		void removeIf(UnaryPre op)
		{
			auto it = begin();
			while (it != end()) {
				if (op(*it)) {
					it = erase(it);
				}
				else {
					++it;
				}
			}
		}

		void reverse() noexcept
		{
			if (size() < 2) {
				return;
			}
			auto beg = begin();
			auto next = beg;
			do {
				beg = next;
				++next;
				using std::swap;
				swap(beg._ptr->_pre, beg._ptr->_next);
			} while (beg._ptr != _pHead);
		}

		void unique()
		{
			unique(std::equal_to<>());
		}

		template<typename BinPre>
		void unique(BinPre equal)
		{
			auto beg = begin();
			auto next = beg;
			++next;
			while (beg != end()) {
				if (equal(*beg, *next)) {
					next = erase(next);
				}
				else {
					beg = next;
					++next;
				}
			}
		}

		void sort()
		{
			_auxSort(begin(), end(), _size, std::less<ValueType>());
		}

		template<typename Comp>
		void sort(Comp comp)
		{
			_auxSort(begin(), end(), _size, comp);
		}

	protected:
		_NodePtr _pHead;
		Alloc _alloc;
		_NodeAlloc _nodeAlloc;
		SizeType _size;

	private:

		// Initialize an empty List
		void _initialize()
		{
			// Construct head node
			_MSTD_TRY
				_NodePtr tmp = _allocateNode();
				tmp->_pre = tmp->_next = tmp;
				_pHead = tmp;
			_MSTD_CATCH_ALL
				throw;
			_MSTD_END_CATCH
		}

		// Allocate a new node
		_NodePtr _allocateNode()
		{
			return AllocatorTraits<_NodeAlloc>::allocate(_nodeAlloc, 1);
		}

		// Construct element value in node
		template<typename... Args>
		void _constructNode(_NodePtr pNode, Args&&... args)
		{			
			AllocatorTraits<Alloc>::construct(
				_alloc, 
				addressof(pNode->_val),
				forward<Args>(args)...
			);
		}

		// Destruct element value
		void _destroyNode(_NodePtr pNode)
		{			
			AllocatorTraits<Alloc>::destroy(_alloc, addressof(pNode->_val));
		}

		// Give back node space
		void _deallocateNode(_NodePtr pNode)
		{
			AllocatorTraits<_NodeAlloc>::deallocate(_nodeAlloc, pNode, 1);
		}

		// Destroy the list
		void _destroy()
		{
			_cleanUp();
			// Free head nodes
			_deallocateNode(_pHead);
		}

		// Set List to the initialized state
		void _cleanUp()
		{
			auto first = begin();
			auto last = end();
			auto next = first;

			// Free all elements and nodes
			while (next != last) {
				++next;
				_destroyNode(first._ptr);
				_deallocateNode(first._ptr);
				first = next;
			}

			// Reset head node
			_pHead->_pre = _pHead->_next = _pHead;
			_size = 0;
		}

		// Insert a node with value {val} before {pos}
		// Return the new node's posission
		_NodePtr _auxInsert(_NodePtr pos, const ValueType &val)
		{
			// Construct the node to be inserted
			_NodePtr tmp = _allocateNode();
			_constructNode(tmp, val);

			// Insert the new node
			tmp->_next = pos;
			tmp->_pre = pos->_pre;
			pos->_pre->_next = tmp;
			pos->_pre = tmp;

			// increase the size counter
			++_size;

			return tmp;
		}

		_NodePtr _auxInsert(_NodePtr pos, ValueType &&val)
		{
			// Construct the node to be inserted
			_NodePtr tmp = _allocateNode();
			_constructNode(tmp, move(val));

			// Insert the new node
			tmp->_next = pos;
			tmp->_pre = pos->_pre;
			pos->_pre->_next = tmp;
			pos->_pre = tmp;

			// increase the size counter
			++_size;

			return tmp;
		}

		// Insert copies of the range [first, last) before pos
		template<typename InputIt>
		_NodePtr _auxInsertRange(_NodePtr pos, InputIt first, InputIt last)
		{
			auto pre = pos->_pre;
			for (; first != last; ++first) {
				_auxInsert(pos, *first);
			}

			return pre->_next;
		}

		// Erase a node from List
		_NodePtr _auxErase(_NodePtr pos)
		{
			if (pos != end()._ptr) {
				auto nextPos = pos->_next;
				pos->_pre->_next = nextPos;
				nextPos->_pre = pos->_pre;

				_destroyNode(pos);
				_deallocateNode(pos);
				--_size;

				return nextPos;
			}
			return pos;
		}

		// Erase a the range [first, last) from List
		_NodePtr _auxEraseRange(_NodePtr first, _NodePtr last)
		{
			while (first != last) {
				first = _auxErase(first);
			}

			return last;
		}

		void _auxResize(SizeType count, const ValueType &val = ValueType())
		{
			if (count < size()) {
				// Shrink List
				while (size() != count) {
					popBack();
				}
			}
			else if (count > size()) {
				// Expand List
				while (size() != count) {
					pushBack(val);
				}
			}
		}

		// Splice range [first, last) at {pos}
		// {pos} is not supposed to belong to this ranges
		void _auxSplice(_NodePtr pos, _NodePtr first, _NodePtr last)
		{
			if (pos != last) {
				auto preFirst = first->_pre;
				// Cut down
				first->_pre = pos->_pre;
				last->_pre->_next = pos;
				// Set this List
				pos->_pre->_next = first;
				pos->_pre = last->_pre;
				// Repair original List
				preFirst->_next = last;
				last->_pre = preFirst;
			}
		}

		template<typename Comp>
		Iterator _auxSort(Iterator first, Iterator last, SizeType size, Comp comp)
		{
			if (size < 2) {
				return first;
			}

			Iterator mid = next(first, size >> 1);
			// Sort [first, mid)
			// and [mid, last)
			first = _auxSort(first, mid, size >> 1, comp);
			mid = _auxSort(mid, last, size - (size >> 1), comp);

			// Merge sorted ranges [first, mid) and [mid, last)
			Iterator retFirst = first;
			bool firstLoop = true;
			while (true) {
				if (comp(*mid, *first)) {
					if (firstLoop) {
						retFirst = mid;
						firstLoop = false;
					}
					splice(first, *this, mid++);
					// Arrived at last
					if (mid == last) {
						return retFirst;
					}
				}
				else {
					++first;
					// Arrived at mid
					if (first == mid) {
						return retFirst;
					}
				}
			}
		}
	};

	template<typename T, typename Alloc>
	bool operator==(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
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
	bool operator!=(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, typename Alloc>
	bool operator<(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
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
	bool operator<=(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
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
	bool operator>(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
	{
		return !(lhs <= rhs);
	}

	template<typename T, typename Alloc>
	bool operator>=(const List<T, Alloc> &lhs, const List<T, Alloc> &rhs)
	{
		return !(lhs < rhs);
	}

	template<typename T, typename Alloc>
	void swap(List<T, alloc> &lhs, List<T, Alloc> &rhs) noexcept
	{
		lhs.swap(rhs);
	}
}