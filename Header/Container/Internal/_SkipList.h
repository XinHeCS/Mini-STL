#pragma once

// Internal use
// Skip List standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <Iterator/Iterator.h>
#include <Container/Vector.h>

// STL header and cpp standard header
#include <initializer_list>
#include <random>
#include <ctime>

namespace MSTD {

	// This class is used in other level of a skip list
	// other than bottom
	template<typename T>
	class _SkipNode
	{
	public:
		using ValueType = T;
		using _NodePtr = _SkipNode * ;
		using Pointer = ValueType * ;

		_NodePtr _forward, _next; // Pointers to the previous and next elem in the same level
		_NodePtr _up, _down; // Pointers to the up level node or down level node
		Pointer _ptr; // Pointer to object stored in this node
		size_t _level; // Level of current _SkipNode

		_SkipNode(const _SkipNode &that) = delete;
		_SkipNode(_SkipNode &&that) = delete;
		_SkipNode& operator=(const _SkipNode &that) = delete;
		_SkipNode& operator=(_SkipNode &&that) = delete;

		// Create a skip node
		template<typename NodeAlloc>
		static _NodePtr createNode(NodeAlloc &alloc, size_t level)
		{
			_NodePtr node = AllocatorTraits<NodeAlloc>::allocate(alloc, 1);
			node->_up = node->_down = nullptr;
			node->_forward = node->_next = nullptr;
			node->_ptr = nullptr;
			node->_level = level;

			return node;
		}

		// Construct object
		template<typename DataAlloc, typename... Args>
		static void constructNode(DataAlloc &alloc, _NodePtr node, Args&&... args)
		{
			node->_ptr = AllocatorTraits<DataAlloc>::allocate(alloc, 1);
			AllocatorTraits<DataAlloc>::construct(alloc, node->_ptr, MSTD::forward<Args>(args)...);
		}

		// Destruct object
		template<typename DataAlloc>
		static void destroyNode(DataAlloc &alloc, _NodePtr node)
		{
			AllocatorTraits<DataAlloc>::destroy(alloc, node->_ptr);
			AllocatorTraits<DataAlloc>::deallocate(alloc, node->_ptr);
		}

		// Deallocate node
		template<typename NodeAlloc>
		static void deallocateNode(NodeAlloc &alloc, _NodePtr node)
		{
			AllocatorTraits<NodeAlloc>::deallocate(alloc, node);
		}
	};

	// Iterator of SkipList
	// Note that Iterator can be in any level
	template<typename _SkipList>
	class _SkipListConstIterator
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;
		using DifferenceType = typename _SkipList::DifferenceType;
		using Pointer = typename _SkipList::ConstPointer;
		using ValueType = typename _SkipList::ValueType;
		using Reference = const ValueType&;

		using _NodePtr = typename _SkipList::_NodePtr;

		_SkipListConstIterator() :
			_cur()
		{}

		_SkipListConstIterator(_NodePtr ptr) :
			_cur(ptr)
		{}

		// Get reference to object inside node
		Reference operator*()
		{
			return *_cur->_ptr;
		}

		// Get pointer to object inside node
		Pointer operator->()
		{
			return _cur->_ptr;
		}

		// Preincrement
		_SkipListConstIterator& operator++()
		{
			// Go down
			while (_cur->_level != 0) {
				_cur = _cur->_down;
			}
			// Go next
			_cur = _cur->_next;
			return *this;
		}

		// Postincrement
		_SkipListConstIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		// Predecrement
		_SkipListConstIterator& operator--()
		{
			// Go down
			while (_cur->_level != 0) {
				_cur = _cur->_down;
			}
			// Go forward
			_cur = _cur->_forward;
			return *this;
		}

		// Postdecrement
		_SkipListConstIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		bool operator==(const _SkipListConstIterator &that)
		{
			return _cur->_ptr == that._cur->_ptr;
		}

		bool operator!=(const _SkipListConstIterator &that)
		{
			return _cur->_ptr != that._cur->_ptr;
		}

		_NodePtr _cur;
	};

	template<typename _SkipList>
	class _SkipListIterator : public _SkipListConstIterator<_SkipList>
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;
		using DifferenceType = typename _SkipList::DifferenceType;
		using Pointer = typename _SkipList::Pointer;
		using ValueType = typename _SkipList::ValueType;
		using Reference = ValueType&;

		using _NodePtr = typename _SkipList::_NodePtr;

		// Constructor
		using _SkipListConstIterator<_SkipList>::_SkipListConstIterator;

		// Get reference to object inside node
		Reference operator*()
		{
			return *_cur->_ptr;
		}

		// Get pointer to object inside node
		Pointer operator->()
		{
			return _cur->_ptr;
		}

		// Preincrement
		_SkipListIterator& operator++()
		{
			// Go down
			while (_cur->_level != 0) {
				_cur = _cur->_down;
			}
			// Go next
			_cur = _cur->_next;
			return *this;
		}

		// Postincrement
		_SkipListIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		// Predecrement
		_SkipListIterator& operator--()
		{
			// Go down
			while (_cur->_level != 0) {
				_cur = _cur->_down;
			}
			// Go forward
			_cur = _cur->_forward;
			return *this;
		}

		// Postdecrement
		_SkipListIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
	};

	template<typename _Config>
	class _SkipList
	{
	public:
		enum _Flags
		{
			_MULTI = _Config::MULTI
		};

		using DifferenceType = typename _Config::DifferenceType;
		using ConstReference = typename _Config::ConstReference;
		using ConstPointer = typename _Config::ConstPointer;
		using ValueType = typename _Config::ValueType;
		using KeyType = typename _Config::KeyType;
		using Reference = typename _Config::Reference;
		using Pointer = typename _Config::Pointer;
		using SizeType = typename _Config::SizeType;
		using KeyCompare = typename _Config::KeyCompare;

		using ConstIterator = _SkipListConstIterator<_SkipList>;
		using Iterator = _SkipListIterator<_SkipList>;
		using ConstReverseIterator = MSTD::ReverseIterator<ConstIterator>;
		using ReverseIterator = MSTD::ReverseIterator<Iterator>;

		using AllocatorType = typename _Config::AllocatorType;
		using _NodeAlloc = MSTD::Allocator<_SkipNode<ValueType>>;

		using _Node = _SkipNode<ValueType>;
		using _NodePtr = _SkipNode<ValueType>*;

		using _PairIB = std::pair<Iterator, bool>;
		using _PairCB = std::pair<ConstIterator, bool>;
		using _RangePtr = std::pair<_NodePtr, _NodePtr>;
		using _RangeIt = std::pair<Iterator, Iterator>;
		using _RangeCIt = std::pair<ConstIterator, ConstIterator>;

		/////////////////////////////////////
		//
		//	Constructors and destructor
		//
		////////////////////////////////////

		_SkipList() :
			_levels(),
			_size(0),
			_comp(),
			_dataAl(),
			_nodeAl()
		{
			_initialize();
		}

		explicit _SkipList(const KeyCompare &comp, 
			const AllocatorType &alloc = AllocatorType()) :
			_levels(),
			_size(0),
			_comp(comp),
			_dataAl(alloc),
			_nodeAl()
		{
			_initialize();
		}

		explicit _SkipList(const AllocatorType &alloc) :
			_SkipList(KeyCompare(), alloc)
		{}

		template<typename InputIt>
		_SkipList(InputIt first, InputIt last, 
					const KeyCompare &comp = KeyCompare(), 
					const AllocatorType &alloc = AllocatorType()) :
			_SkipList(comp, alloc)
		{
			while (first != last) {
				_auxInsert(*first++);
			}
		}

		_SkipList(const _SkipList &that) :
			_SkipList(that.begin(), that.end())
		{}

		_SkipList(_SkipList &&that) noexcept :
			_levels(MSTD::move(that._levels)),
			_nil(MSTD::move(that._nil)),
			_size(MSTD::move(that._size)),
			_comp(MSTD::move(that._comp)),
			_dataAl(MSTD::move(that._dataAl)),
			_nodeAl(MSTD::move(that._nodeAl))
		{
			that._nil = nullptr;
			that._size = 0;
		}

		_SkipList(std::initializer_list<ValueType> il,
					const KeyCompare &comp = KeyCompare(),
					const AllocatorType &alloc = AllocatorType()) :
			_SkipList(il.begin(), il.end(), comp, alloc)
		{}

		~_SkipList()
		{
			_destroy();
		}

		_SkipList& operator=(const _SkipList &that)
		{
			if (this != MSTD::addressof(that)) {
				_cleanUp();

				_comp = that._comp;
				_dataAl = that._dataAl;
				_nodeAl = that._nodeAl;

				_initialize();
				for (auto &&val : that) {
					_NodePtr node = _Node::createNode(_nodeAl, 0);
					_Node::constructNode(_dataAl, node, val);
					_auxInsert(node);
				}
			}
			return *this;
		}

		_SkipList& operator=(_SkipList &&that) noexcept
		{
			if (this != MSTD::addressof(that)) {
				_levels = MSTD::move(that._levels);
				_nil = MSTD::move(that._nil);
				_size = MSTD::move(that._size);
				_comp = MSTD::move(that._comp);
				_dataAl = MSTD::move(that._dataAl);
				_nodeAl = MSTD::move(that._nodeAl);
				that._nil = nullptr;
				that._size = 0;
			}
			return *this;
		}

		_SkipList& operator=(std::initializer_list<ValueType> il)
		{
			clear();
			for (auto &&val : il) {
				_NodePtr node = _Node::createNode(_nodeAl, 0);
				_Node::constructNode(_dataAl, node, val);
				_auxInsert(node);
			}
			return *this;
		}

		/////////////////////////////////////
		//
		//			Iterators
		//
		/////////////////////////////////////

		Iterator begin() noexcept
		{
			return Iterator(_levels.front()->_next);
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(_levels.front()->_next);
		}

		ConstIterator cbegin() const noexcept
		{
			return ConstIterator(_levels.front()->_next);
		}

		Iterator end() noexcept
		{
			return Iterator(_nil);
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(_nil);
		}

		ConstIterator cend() const noexcept
		{
			return ConstIterator(_nil);
		}

		ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(_nil);
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(_nil);
		}

		ConstReverseIterator crbegin() const noexcept
		{
			return ConstReverseIterator(_nil);
		}

		ReverseIterator rend() noexcept
		{
			return ReverseIterator(_levels.front()->_next);
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(_levels.front()->_next);
		}

		ConstReverseIterator crend() const noexcept
		{
			return ConstReverseIterator(_levels.front()->_next);
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

		SizeType size() const
		{
			return _size;
		}

		/////////////////////////////////////
		//
		//			Modifiers
		//
		/////////////////////////////////////

		void clear() noexcept
		{
			_cleanUp();
			_initialize();
		}

		Iterator erase(ConstIterator pos)
		{
			return Iterator(_auxErase(pos._cur));
		}

		Iterator erase(ConstIterator first, ConstIterator last)
		{
			while (first != last) {
				_auxErase((first++)._cur);
			}
			return Iterator(last._cur);
		}

		SizeType erase(const KeyType &key)
		{
			_RangePtr range = _auxEqualRange(key);
			SizeType cnt = MSTD::distance(Iterator(range.first), Iterator(range.second));
			erase(range.first, range.second);
			return cnt;
		}

		_PairIB insert(const ValueType &val)
		{
			return _auxInsert(val);
		}

		_PairIB insert(ValueType &&val)
		{
			return _auxInsert(MSTD::move(val));
		}

		template<typename InputIt>
		void insert(InputIt first, InputIt last)
		{
			while (first != last) {
				insert(*first++);
			}
		}

		void insert(std::initializer_list<ValueType> il)
		{
			insert(il.begin(), il.end());
		}

		template<typename... Args>
		_PairIB emplace(Args&&... args)
		{
			_NodePtr node = _Node::createNode(_nodeAl, 0);
			_Node::constructNode(_dataAl, node, MSTD::forward<Args>(args)...);
			
			// Find insertion position
			_NodePtr pos = _search(_getKeyFromNode(node));

			// Empty and insert directly
			if (!pos) {
				_requireLevel(0);
				pos = _levels.front();
				// Insert
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}

			if (this->_MULTI) {
				// Insert
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}
			else {
				// Check whether this object is unique
				if (!pos->_ptr ||
					_comp(
						_getKeyFromNode(pos),
						_getKeyFromVal(node)
					)) { // Unique element
					// Insert
					_insertNodeAt(pos, node);
					// Level up
					_levelUp(node);
					++_size;
					return _PairIB(Iterator(node), true);
				}
				else {
					// duplicated
					// free node
					_Node::destroyNode(_dataAl, node);
					_Node::deallocateNode(_nodeAl, node);
					return _PairIB(Iterator(pos), false);
				}
			}
		}

		void swap(_SkipList &that) noexcept
		{			
			using std::swap;
			_levels.swap(that._levels);
			swap(_nil, that._nil);
			swap(_size, that._size);
			swap(_comp, that._comp);
			swap(_dataAl, that._dataAl);
			swap(_nodeAl, that._nodeAl);
		}

		/////////////////////////////////////
		//
		//			Lookup
		//
		/////////////////////////////////////

		Iterator find(const KeyType &key)
		{
			return Iterator(_auxFind(key));
		}

		ConstIterator find(const KeyType &key) const
		{
			return Iterator(_auxFind(key));
		}

		_RangeIt equalRange(const KeyType &key)
		{
			return _RangeIt(lowerBound(key), upperBound(key));
		}

		_RangeCIt equalRange(const KeyType &key) const
		{
			return _RangeCIt(lowerBound(key), upperBound(key));
		}

		Iterator lowerBound(const KeyType &key)
		{
			return Iterator(_auxLower(key));
		}

		ConstIterator lowerBound(const KeyType &key) const
		{
			return ConstIterator(_auxLower(key));
		}

		Iterator upperBound(const KeyType &key)
		{
			return Iterator(_auxUpper(key));
		}

		ConstIterator upperBound(const KeyType &key) const
		{
			return ConstIterator(_auxUpper(key));
		}

		SizeType count(const KeyType &key) const
		{
			_RangeCIt range = equalRange(key);
			return MSTD::distance(range.first, range.second);
		}

		bool contains(const KeyType &key) const
		{
			_NodePtr pos = _auxFind(key);
			return !_isEnd(pos);
		}

	private:

		// Static members
		static std::default_random_engine e;
		static std::uniform_int_distribution<unsigned> u;

		void _destroy() noexcept
		{
			_cleanUp();
			_Node::deallocateNode(_nodeAl, _nil);
		}

		void _cleanUp() noexcept
		{
			for (auto header : _levels) {
				_NodePtr cur = header->_next;
				while (!_isEnd(cur)) {
					_NodePtr next = cur->_next;
					if (cur->_level == 0) {
						_Node::destroyNode(_dataAl, cur);
					}
					_Node::deallocateNode(_nodeAl, cur);
					cur = next;
				}
				_Node::deallocateNode(_nodeAl, header);
			}
			_size = 0;
			_levels.clear();
		}

		void _initialize()
		{
			// Create nil node for end() pos
			_nil = _Node::createNode(_nodeAl, 0);
			_requireLevel(0);
		}

		bool _isLevelUp() const
		{
			// Level up a object with the probability of  1 / 2
			// And here we didn't limit the max level here
			return u(e) ? true : false;
		}

		// Determine whether node has gotten to the end of any list
		bool _isEnd(_NodePtr node) const
		{
			return node == _nil;
		}

		SizeType _getLevelSize() const
		{
			return _levels.size();
		}

		// Search for the element of Node
		// If the elem dosen't exist in the skip list
		// then return the position where it can be inserted in.
		_NodePtr _search(const KeyType &key)
		{
			if (_levels.empty()) {
				return nullptr;
			}
			_NodePtr tryNode = _levels.back();
			_NodePtr pos = _levels.back();
			while (tryNode) {
				if (!_isEnd(tryNode->_next) &&
					!_comp(
						key,
						_getKeyFromNode(tryNode->_next)
				)) { // Go next
					// Record original position
					pos = tryNode;
					tryNode = tryNode->_next;
				}
				else { // Go down
					// Record original position
					pos = tryNode;
					tryNode = tryNode->_down;
				}
			}
			return pos;
		}

		const KeyType& _getKeyFromNode(_NodePtr node) const
		{
			return _getKeyFromVal(*node->_ptr);
		}

		const KeyType& _getKeyFromVal(const ValueType &val) const
		{
			return _Config::getKeyFromVal(val);
		}

		// if {level} exceeds the size of current level,
		// then create a new level
		void _requireLevel(SizeType level)
		{
			if (level + 1 > _getLevelSize()) {
				_NodePtr levelHeader = _Node::createNode(_nodeAl, level);
				levelHeader->_next = _nil;
				if (level > 0) {// Set pointer to lower level, except the first level
					levelHeader->_down = _levels[level - 1];
					levelHeader->_down->_up = levelHeader;
				}
				else { // Initialize the first level
					levelHeader->_next->_forward = levelHeader;
				}
				_levels.pushBack(levelHeader);
			}
		}

		// Promote the node a level
		void _levelUp(_NodePtr node)
		{
			while (_isLevelUp()) {
				_NodePtr upNode = _Node::createNode(_nodeAl, node->_level + 1);
				upNode->_ptr = node->_ptr;
				upNode->_down = node;
				node->_up = upNode;
				_requireLevel(upNode->_level);
				
				// Find nearest node in the upper level
				_NodePtr upPre = node->_forward;
				while (!upPre->_up) {
					upPre = upPre->_forward;
				}
				upPre = upPre->_up;
				// Insert at upper level
				_insertNodeAt(upPre, upNode);
				// Reset node to restart again
				node = upNode;
			}
		}

		// Insert {node} after {pos}
		// This function deal with the situation when
		// insertion occurs at the position bwtween 
		// the last node of bottom level and _nil.
		//		e.g.
		//										[newNode]: Promotion and needn't modify {_nil}
		//											  |    here.
		//											 \|/    
		//			(level : 1) ... <----> [node2] - - - - -
		//												     \	   
		//												       \    
		//											 [newNode]   \    
		//												  |        \    
		//												 \|/  		|    
		//			 (level : 0) ... <----> [node1] <-----------> |_nil|
		// In this situation, we should relink {_nil}'s {_forward} pointer
		// to ensure {_nil} remains the property of end iterator
		void _insertNodeAt(_NodePtr pos, _NodePtr node)
		{
			// Insert
			node->_forward = pos;
			node->_next = pos->_next;
			if (!_isEnd(pos->_next) || node->_level == 0) {
				pos->_next->_forward = node;
			}
			pos->_next = node;
		}

		_PairIB _auxInsert(const ValueType &val)
		{
			// Find insertion position
			_NodePtr pos = _search(_getKeyFromVal(val));

			// Empty and insert directly
			if (!pos) {
				_requireLevel(0);
				pos = _levels.front();
				// Insert
				_NodePtr node = _Node::createNode(_nodeAl, 0);
				_Node::constructNode(_dataAl, node, (val));
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}
			
			if (this->_MULTI) {
				// Insert
				_NodePtr node = _Node::createNode(_nodeAl, 0);
				_Node::constructNode(_dataAl, node, (val));
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}
			else {
				// Check whether this object is unique
				if (!pos->_ptr ||
					_comp(
						_getKeyFromNode(pos),
						_getKeyFromVal(val)
				)) { // Unique element
					// Insert
					_NodePtr node = _Node::createNode(_nodeAl, 0);
					_Node::constructNode(_dataAl, node, (val));
					_insertNodeAt(pos, node);
					// Level up
					_levelUp(node);
					++_size;
					return _PairIB(Iterator(node), true);
				}
				else {
					// duplicated
					return _PairIB(Iterator(pos), false);
				}
			}
		}

		_PairIB _auxInsert(ValueType &&val)
		{
			// Find insertion position
			_NodePtr pos = _search(_getKeyFromVal(val));

			// Empty and insert directly
			if (!pos) {
				_requireLevel(0);
				pos = _levels.front();
				// Insert
				_NodePtr node = _Node::createNode(_nodeAl, 0);
				_Node::constructNode(_dataAl, node, MSTD::move(val));
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}

			if (this->_MULTI) {
				// Insert
				_NodePtr node = _Node::createNode(_nodeAl, 0);
				_Node::constructNode(_dataAl, node, MSTD::move(val));
				_insertNodeAt(pos, node);
				// Level up
				_levelUp(node);
				++_size;
				return _PairIB(Iterator(node), true);
			}
			else {
				// Check whether this object is unique
				if (!pos->_ptr ||
					_comp(
						_getKeyFromNode(pos),
						_getKeyFromVal(val)
					)) { // Unique element
						// Insert
					_NodePtr node = _Node::createNode(_nodeAl, 0);
					_Node::constructNode(_dataAl, node, MSTD::move(val));
					_insertNodeAt(pos, node);
					// Level up
					_levelUp(node);
					++_size;
					return _PairIB(Iterator(node), true);
				}
				else {
					// duplicated
					return _PairIB(Iterator(pos), false);
				}
			}
		}

		// Delete node {pos}
		// Assume that pos has been located at 
		// bottom level
		_NodePtr _auxErase(_NodePtr pos)
		{
			_NodePtr ret = pos->_next;
			// Destroy object
			_Node::destroyNode(_dataAl, pos);
			--_size;
		
			// Delete pos from bottom to top
			while (pos) {
				_NodePtr up = pos->_up;
				pos->_forward->_next = pos->_next;
				pos->_next->_forward = pos->_forward;
				_Node::deallocateNode(_nodeAl, pos);
				pos = up;
			}
			return ret;
		}

		_NodePtr _auxFind(const KeyType &key)
		{
			_NodePtr pos = _auxLower(key);
			return (_isEnd(pos) || _comp(key, _getKeyFromNode(pos)) ?
					_nil : pos);
		}

		_NodePtr _auxLower(const KeyType &key)
		{
			_NodePtr tryNode = _levels.back();
			_NodePtr beg = tryNode;
			while (tryNode) {
				if (!_isEnd(tryNode->_next) &&
					_comp(_getKeyFromNode(tryNode->_next), key)) {
					// Go next
					beg = tryNode;
					tryNode = tryNode->_next;
				}
				else {
					// Go down
					beg = tryNode;
					tryNode = tryNode->_down;
				}
			}
			return beg->_next;
		}

		_NodePtr _auxUpper(const KeyType &key)
		{
			_NodePtr tryNode = _levels.back();
			_NodePtr end = tryNode;
			while (tryNode) {
				if (!_isEnd(tryNode->_next) &&
					!_comp(key, _getKeyFromNode(tryNode->_next))) {
					// Go next
					end = tryNode;
					tryNode = tryNode->_next;
				}
				else {
					// Go down
					end = tryNode;
					tryNode = tryNode->_down;
				}
			}
			return end->_next;
		}

		_RangePtr _auxEqualRange(const KeyType &key)
		{
			return _RangePtr(_auxLower(key), _auxUpper(key));
		}

	protected:
		Vector<_NodePtr> _levels; // Store all header pointers of each level
		SizeType _size; // Number of elements in skip list
		_NodePtr _nil; // Dummy node for nil
		KeyCompare _comp;
		AllocatorType _dataAl;
		_NodeAlloc _nodeAl;
	};

	template<typename _Config>
	std::default_random_engine _SkipList<_Config>::e(static_cast<unsigned int>(std::time(0)));

	template<typename _Config>
	std::uniform_int_distribution<unsigned> _SkipList<_Config>::u(0, 1);


}

