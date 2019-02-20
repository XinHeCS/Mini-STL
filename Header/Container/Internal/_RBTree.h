#pragma once

// Internal use
// RB-Tree standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <Iterator/Iterator.h>
#include <Container/Internal/_BaseTree.h>

// STL header and cpp standard header
#include <initializer_list>
#include <utility>
#include <stdexcept>

namespace MSTD {

	template<typename T>
	class _TreeNode // Tree node template class
	{
	public:
		using _NodePtr = _TreeNode * ;
		using ValueType = T;
		// Color enum var
		enum _Color
		{
			RED,
			BLACK
		};
		// Data members
		_NodePtr _left; // Left child of this node
		_NodePtr _right; // Right child of this node
		_NodePtr _parent; // Parent node of this node
		_Color _color;	// Color of this node
		ValueType _val; // Object stored in this node
		bool _isNil; // To identify whether this node is header
					// (true means yes and false otherwise)

		// Useful utilities for _TreeNode
		template<typename NodeAlloc>
		static _NodePtr createNode(NodeAlloc &alloc)
		{
			_NodePtr pNode = AllocatorTraits<NodeAlloc>::allocate(alloc, 1);
			pNode->_left = pNode->_right = pNode->_parent = pNode;
			pNode->_color = RED;
			pNode->_isNil = false;
			return pNode;
		}

		// Construct element of node
		template<typename DataAlloc, typename... Args>
		static void constructNode(DataAlloc &alloc, _NodePtr pNode, Args&&... args)
		{
			AllocatorTraits<DataAlloc>::construct(alloc, MSTD::addressof(pNode->_val), MSTD::forward<Args>(args)...);
		}

		// Deconstruct element of node
		template<typename DataAlloc>
		static void destroyNode(DataAlloc &alloc, _NodePtr pNode)
		{
			AllocatorTraits<DataAlloc>::destroy(alloc, MSTD::addressof(pNode->_val));
		}

		// Deallocate node
		template<typename NodeAlloc>
		static void freeNode(NodeAlloc &alloc, _NodePtr pNode)
		{
			AllocatorTraits<NodeAlloc>::deallocate(alloc, pNode, 1);
		}
	};

	template<typename _Tree>
	class _RBTreeConstIterator
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;
		using DifferenceType = typename _Tree::DifferenceType;
		using Pointer = typename _Tree::ConstPointer;
		using ValueType = typename _Tree::ValueType;
		using Reference = const ValueType&;

		//using _NodePtr = _TreeNode<ValueType>*;
		using _NodePtr = typename _Tree::_NodePtr;

		_RBTreeConstIterator() :
			_cur()
		{}

		_RBTreeConstIterator(_NodePtr ptr) :
			_cur(ptr)
		{}

		// Dereference to get value
		Reference operator*()
		{
			return _cur->_val;
		}

		// Return pointer to object
		Pointer operator->()
		{
			return MSTD::addressof(operator*());
		}

		// Preincrenment
		_RBTreeConstIterator& operator++()
		{
			if (!_cur->_right->_isNil) {
				// Find left most node of right child
				_NodePtr pNode = _cur->_right;
				while (!pNode->_left->_isNil) {
					pNode = pNode->_left;
				}
				_cur = pNode;
			}
			else {
				// Looking up to find root node of current 
				// right subtree
				_NodePtr pNode;
				while (!(pNode = _cur->_parent)->_isNil && pNode->_right == _cur) {
					_cur = pNode;
				}
				_cur = pNode;
			}
			return *this;
		}

		// Postincrement
		_RBTreeConstIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		// Predecrement
		_RBTreeConstIterator& operator--()
		{
			if (!_cur->_left->_isNil) {
				// Find right most node of left child
				_NodePtr pNode = _cur->_left;
				while (!pNode->_right->_isNil) {
					pNode = pNode->_right;
				}
				_cur = pNode;
			}
			else {
				// Looking up to find root node of 
				// current left subtree
				_NodePtr pNode;
				while (!(pNode = _cur->_parent)->_isNil && pNode->_left == _cur) {
					_cur = pNode;
				}
				_cur = pNode;
			}
			return *this;
		}

		// Postdecrement
		_RBTreeConstIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		bool operator==(const _RBTreeConstIterator &that)
		{
			return _cur == that._cur;
		}

		bool operator!=(const _RBTreeConstIterator &that)
		{
			return !(*this == that);
		}
	
		_NodePtr _cur;
	};

	template<typename _Tree>
	class _RBTreeIterator : public _RBTreeConstIterator<_Tree>
	{
	public:
		using IteratorCategory = BidirectionalIteratorTag;
		using DifferenceType = typename _Tree::DifferenceType;
		using Pointer = typename _Tree::Pointer;
		using ValueType = typename _Tree::ValueType;
		using Reference = ValueType&;
		
		using _NodePtr = typename _Tree::_NodePtr;

		// Constructor
		using _RBTreeConstIterator<_Tree>::_RBTreeConstIterator;

		// Dereference to get value
		Reference operator*()
		{
			return _cur->_val;
		}

		// Return pointer to object
		Pointer operator->()
		{
			return MSTD::addressof(operator*());
		}

		// Preincrenment
		_RBTreeIterator& operator++()
		{
			if (!_cur->_right->_isNil) {
				// Find left most node of right child
				_NodePtr pNode = _cur->_right;
				while (!pNode->_left->_isNil) {
					pNode = pNode->_left;
				}
				_cur = pNode;
			}
			else {
				// Looking up to find root node of current 
				// right subtree
				_NodePtr pNode;
				while (!(pNode = _cur->_parent)->_isNil && pNode->_right == _cur) {
					_cur = pNode;
				}
				_cur = pNode;
			}
			return *this;
		}

		// Postincrement
		_RBTreeIterator operator++(int)
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		// Predecrement
		_RBTreeIterator& operator--()
		{
			if (!_cur->_left->_isNil) {
				// Find right most node of left child
				_NodePtr pNode = _cur->_left;
				while (!pNode->_right->_isNil) {
					pNode = pNode->_right;
				}
				_cur = pNode;
			}
			else {
				// Looking up to find root node of 
				// current left subtree
				_NodePtr pNode;
				while (!(pNode = _cur->_parent)->_isNil && pNode->_left == _cur) {
					_cur = pNode;
				}
				_cur = pNode;
			}
			return *this;
		}

		// Postdecrement
		_RBTreeIterator operator--(int)
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
	};

	// Red black tree template class
	template<typename _ConfigParam>
	class _RBTree 
		: public _BaseTree<_ConfigParam, _TreeNode<typename _ConfigParam::ValueType>>
	{
	public:
		enum _Flags
		{
			_MULTI = _ConfigParam::MULTI
		};

		using DifferenceType = typename _ConfigParam::DifferenceType;
		using ConstReference = typename _ConfigParam::ConstReference;
		using ConstPointer = typename _ConfigParam::ConstPointer;
		using ValueType = typename _ConfigParam::ValueType;
		using KeyType = typename _ConfigParam::KeyType;
		using Reference = typename _ConfigParam::Reference;
		using Pointer = typename _ConfigParam::Pointer;
		using SizeType = typename _ConfigParam::SizeType;
		using KeyCompare = typename _ConfigParam::KeyCompare;
		
		using ConstIterator = _RBTreeConstIterator<_RBTree>;
		using Iterator = _RBTreeIterator<_RBTree>;
		using ConstReverseIterator = MSTD::ReverseIterator<ConstIterator>;
		using ReverseIterator = MSTD::ReverseIterator<Iterator>;

		using AllocatorType = typename _ConfigParam::AllocatorType;
		using _NodeAlloc = Allocator<_TreeNode<ValueType>>;

		using _Node = _TreeNode<ValueType>;
		using _NodePtr = _TreeNode<ValueType>*;

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

		_RBTree() :
			_BaseTree<_ConfigParam, _Node>(),
			_comp(),
			_dataAl(),
			_nodeAl()
		{
			_initialize();
		}

		explicit _RBTree(const KeyCompare &comp, 
			const AllocatorType &alloc = AllocatorType()) :
			_BaseTree<_ConfigParam, _Node>(),
			_comp(comp),
			_dataAl(alloc),
			_nodeAl()
		{
			_initialize();
		}

		explicit _RBTree(const AllocatorType &alloc) :
			_RBTree(KeyCompare(), alloc)
		{}

		template<typename InputIt>
		_RBTree(InputIt first, InputIt last,
				const KeyCompare &comp = KeyCompare(),
				const AllocatorType &alloc = AllocatorType()) :
			_RBTree(comp, alloc)
		{
			while (first != last) {
				_auxInsert(*first++);
			}
		}

		_RBTree(const _RBTree &that) :
			_RBTree(that._comp, that._dataAl)
		{
			this->_root() = this->_copyTree(that._root());
			this->_root()->_parent = this->_NIL();
			this->_size = that._size;
			if (!this->_root()->_isNil) {
				// Set left most and right most
				this->_leftMost() = this->_min(this->_root());
				this->_rightMost() = this->_max(this->_root());
			}
		}

		_RBTree(_RBTree &&that) noexcept :
			_BaseTree<_ConfigParam, _Node>(),
			_comp(move(that._comp)),
			_dataAl(move(that._dataAl)),
			_nodeAl(move(that._nodeAl))
		{
			_initialize();
			this->_head = move(that._head);
			this->_size = move(that._size);
			that._head = nullptr;
			that._size = 0;
		}

		_RBTree(std::initializer_list<ValueType> il,
				const KeyCompare &comp = KeyCompare(),
				const AllocatorType &alloc = AllocatorType()) :
			_RBTree(il.begin(), il.end(), comp, alloc)
		{}

		~_RBTree()
		{
			if (this->_head) {
				_destroy();
			}
		}

		_RBTree& operator=(const _RBTree &that)
		{
			if (this != addressof(that)) {
				clear();
				this->_comp = that._comp;
				this->_dataAl = that._dataAl;
				this->_nodeAl = that._nodeAl;
				this->_root() = this->_copyTree(that._root());
				this->_root()->_parent = this->_NIL();
				this->_size = that._size;
				if (!this->_root()->_isNil) {
					// Set left most and right most
					this->_leftMost() = this->_min(this->_root());
					this->_rightMost() = this->_max(this->_root());
				}
			}
			return *this;
		}

		_RBTree& operator=(_RBTree &&that) noexcept
		{
			if (this != addressof(that)) {
				clear();
				// Stealing tree
				this->_comp = move(that._comp);
				this->_dataAl = move(that._dataAl);
				this->_nodeAl = move(that._nodeAl);
				this->_head = move(that._head);
				this->_size = move(that._size);
				that._head = nullptr;
				that._size = 0;
			}
			return *this;
		}

		_RBTree& operator=(std::initializer_list<ValueType> il)
		{
			clear();
			for (auto &&val : il) {
				_auxInsert(val);
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
			return Iterator(this->_leftMost());
		}

		ConstIterator begin() const noexcept
		{
			return ConstIterator(this->_leftMost());
		}

		ConstIterator cbegin() const noexcept
		{
			return ConstIterator(this->_leftMost());
		}

		Iterator end() noexcept
		{
			return Iterator(this->_head);
		}

		ConstIterator end() const noexcept
		{
			return ConstIterator(this->_head);
		}

		ConstIterator cend() const noexcept
		{
			return ConstIterator(this->_head);
		}

		ReverseIterator rbegin() noexcept
		{
			return ReverseIterator(this->_head);
		}

		ConstReverseIterator rbegin() const noexcept
		{
			return ConstReverseIterator(this->_head);
		}

		ConstReverseIterator crbegin() const noexcept
		{
			return ConstReverseIterator(this->_head);
		}

		ReverseIterator rend() noexcept
		{
			return ReverseIterator(this->_leftMost());
		}

		ConstReverseIterator rend() const noexcept
		{
			return ConstReverseIterator(this->_leftMost());
		}

		ConstReverseIterator crend() const noexcept
		{
			return ConstReverseIterator(this->_leftMost());
		}

		/////////////////////////////////////
		//
		//			Capacity
		//
		/////////////////////////////////////		

		bool empty() const noexcept
		{
			return begin() == end();
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
			_cleanUp(this->_root());
			// Reset root to NIL
			this->_root() = this->_NIL();
			this->_leftMost() = this->_NIL();
			this->_rightMost() = this->_NIL();
			this->_size = 0;
		}

		Iterator erase(ConstIterator pos)
		{
			return Iterator(_auxErase(pos._cur));
		}

		Iterator erase(ConstIterator first, ConstIterator last)
		{
			while (first != last) {
				erase(first++);
			}
			return Iterator(last._cur);
		}

		SizeType erase(const KeyType &key)
		{
			auto range = equalRange(key);
			SizeType cnt = MSTD::distance(range.first, range.second);
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
			_NodePtr insertNode = _Node::createNode(this->_nodeAl);
			_Node::constructNode(this->_dataAl, insertNode, forward<Args>(args)...);

			_NodePtr tryNode = this->_root();
			_NodePtr pos = this->_head;
			bool addLeft = true;
			while (!tryNode->_isNil) {
				pos = tryNode;
				addLeft = _comp(
					_getKeyFromNode(insertNode),
					_getKeyFromNode(pos)
				);
				tryNode = addLeft ? tryNode->_left : tryNode->_right;
			}

			if (this->_MULTI) {
				this->_insertAt(insertNode, pos, addLeft);
				_insertionRebalance(insertNode);

				return _PairIB(Iterator(insertNode), true);
			}
			else {
				// Check if unique
				Iterator where(pos);
				if (addLeft) {
					if (where._cur == this->_leftMost()) {
						// No more predecessor nodes
						this->_insertAt(insertNode, pos, addLeft);
						_insertionRebalance(insertNode);

						return _PairIB(Iterator(insertNode), true);
					}
					else {
						--where;
					}
				}
				if (_comp(_getKeyFromNode(where._cur), _getKeyFromNode(insertNode))) {
					// Unique in the tree
					this->_insertAt(insertNode, pos, addLeft);
					_insertionRebalance(insertNode);

					return _PairIB(Iterator(insertNode), true);
				}

				// Not unique, stop insertion
				// free node
				_Node::destroyNode(this->_dataAl, insertNode);
				_Node::freeNode(this->_nodeAl, insertNode);
				return _PairIB(where, false);
			}
		}

		void swap(_RBTree &that) noexcept
		{
			using std::swap;
			swap(_head, that._head);
			swap(_size, that._size);
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
			return ConstIterator(_auxFind(key));
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
			auto it = find(key);
			return it == end();
		}

	private:
		const KeyType& _getKeyFromNode(_NodePtr node) const
		{
			return _getKeyFromVal(node->_val);
		}
		
		const KeyType& _getKeyFromVal(const ValueType &val) const
		{
			return _ConfigParam::getKeyFromVal(val);
		}

		void _initialize()
		{
			this->_head = _Node::createNode(_nodeAl);
			this->_head->_color = _Node::BLACK;
			this->_head->_isNil = true;
		}

		// Rebalance rb-tree in the insertion place
		void _insertionRebalance(_NodePtr pos)
		{
			while (pos->_parent->_color == _Node::RED) {
				// Left insertion cases
				// Case 1: Red uncle node and left child itself
				if (pos->_parent == pos->_parent->_parent->_left) {
					if (pos->_parent->_parent->_right->_color == _Node::RED) {
						// Recoloring
						pos->_parent->_color = _Node::BLACK;
						pos->_parent->_parent->_right->_color = _Node::BLACK;
						pos->_parent->_parent->_color = _Node::RED;
						pos = pos->_parent->_parent;
					}
					else {
						// Case 2: Black uncle node and right child itself
						if (pos == pos->_parent->_right) {
							pos = pos->_parent;
							_leftRotate(pos);
						}
						// Case 3: Black uncle node and left child itself
						// Recoloring and rotation to keep balance
						pos->_parent->_parent->_color = _Node::RED;
						pos->_parent->_color = _Node::BLACK;
						_rightRotate(pos->_parent->_parent);
					}
				}
				else {
					// Right insertion cases
					// Case 1: Red uncle node and right child itself
					if (pos->_parent == pos->_parent->_parent->_right) {
						if (pos->_parent->_parent->_left->_color == _Node::RED) {
							// Recoloring
							pos->_parent->_color = _Node::BLACK;
							pos->_parent->_parent->_left->_color = _Node::BLACK;
							pos->_parent->_parent->_color = _Node::RED;
							pos = pos->_parent->_parent;
						}
						else {
							// Case 2: Black uncle node and left child itself
							if (pos == pos->_parent->_left) {
								pos = pos->_parent;
								_rightRotate(pos);
							}
							// Case 3: Black uncle node and right child itself
							// Recoloring and rotation to keep balance
							pos->_parent->_parent->_color = _Node::RED;
							pos->_parent->_color = _Node::BLACK;
							_leftRotate(pos->_parent->_parent);
						}
					}
				}
			}
			// Recolor root
			this->_root()->_color = _Node::BLACK;
		}

		// Insert the val according to keyCompare
		_PairIB _auxInsert(const ValueType& val)
		{
			_NodePtr tryNode = this->_root();
			_NodePtr pos = this->_head;
			bool addLeft = true;
			while (!tryNode->_isNil) {
				pos = tryNode;
				addLeft = _comp(
					_getKeyFromVal(val),
					_getKeyFromNode(pos)
				);
				tryNode = addLeft ? tryNode->_left : tryNode->_right;
			}
			
			if (this->_MULTI) {
				_NodePtr insertNode = _Node::createNode(this->_nodeAl);
				_Node::constructNode(this->_dataAl, insertNode, val);
				this->_insertAt(insertNode, pos, addLeft);
				_insertionRebalance(insertNode);

				return _PairIB(Iterator(insertNode), true);
			}
			else {
				// Check if unique
				Iterator where(pos);
				if (addLeft) {
					if (where._cur == this->_leftMost()) {
						// No more predecessor nodes
						_NodePtr insertNode = _Node::createNode(this->_nodeAl);
						_Node::constructNode(this->_dataAl, insertNode, val);
						this->_insertAt(insertNode, pos, addLeft);
						_insertionRebalance(insertNode);

						return _PairIB(Iterator(insertNode), true);
					}
					else {
						--where;
					}
				}
				if (_comp(_getKeyFromNode(where._cur), _getKeyFromVal(val))) {
					// Unique in the tree
					_NodePtr insertNode = _Node::createNode(this->_nodeAl);
					_Node::constructNode(this->_dataAl, insertNode, val);
					this->_insertAt(insertNode, pos, addLeft);
					_insertionRebalance(insertNode);

					return _PairIB(Iterator(insertNode), true);
				}

				// Not unique, stop insertion
				return _PairIB(where, false);
			}
		}

		// Insert the val according to keyCompare
		_PairIB _auxInsert(ValueType&& val)
		{
			_NodePtr tryNode = this->_root();
			_NodePtr pos = this->_head;
			bool addLeft = true;
			while (!tryNode->_isNil) {
				pos = tryNode;
				addLeft = _comp(
					_getKeyFromVal(val),
					_getKeyFromNode(pos)
				);
				tryNode = addLeft ? tryNode->_left : tryNode->_right;
			}

			if (this->_MULTI) {
				_NodePtr insertNode = _Node::createNode(this->_nodeAl);
				_Node::constructNode(this->_dataAl, insertNode, MSTD::move(val));
				this->_insertAt(insertNode, pos, addLeft);
				_insertionRebalance(insertNode);

				return _PairIB(Iterator(insertNode), true);
			}
			else {
				// Check if unique
				Iterator where(pos);
				if (addLeft) {
					if (where._cur == this->_leftMost()) {
						// No more predecessor nodes
						_NodePtr insertNode = _Node::createNode(this->_nodeAl);
						_Node::constructNode(this->_dataAl, insertNode, MSTD::move(val));
						this->_insertAt(insertNode, pos, addLeft);
						_insertionRebalance(insertNode);

						return _PairIB(Iterator(insertNode), true);
					}
					else {
						--where;
					}
				}
				if (_comp(_getKeyFromNode(where._cur), _getKeyFromVal(val))) {
					// Unique in the tree
					_NodePtr insertNode = _Node::createNode(this->_nodeAl);
					_Node::constructNode(this->_dataAl, insertNode, MSTD::move(val));
					this->_insertAt(insertNode, pos, addLeft);
					_insertionRebalance(insertNode);

					return _PairIB(Iterator(insertNode), true);
				}

				// Not unique, stop insertion
				return _PairIB(where, false);
			}
		}

		_NodePtr _copyTree(_NodePtr root)
		{
			if (root->_isNil) {
				return this->_NIL();
			}
			_NodePtr newRoot = _Node::createNode(_nodeAl);
			_Node::constructNode(_dataAl, newRoot, root->_val);
			newRoot->_color = root->_color;

			newRoot->_left = _copyTree(root->_left);
			if (!newRoot->_left->_isNil) {
				newRoot->_left->_parent = newRoot;
			}
			newRoot->_right = _copyTree(root->_right);
			if (!newRoot->_right->_isNil) {
				newRoot->_right->_parent = newRoot;
			}

			return newRoot;
		}

		// Clean up subtree
		void _cleanUp(_NodePtr root) noexcept
		{
			if (!root->_isNil) {
				// Destroy left and right subtrees
				_cleanUp(root->_left);
				_cleanUp(root->_right);
				// Destroy current node
				_Node::destroyNode(this->_dataAl, root);
				_Node::freeNode(this->_nodeAl, root);
				--this->_size;
			}
		}

		void _destroy()
		{
			_cleanUp(this->_root());
			// Free head node
			_Node::freeNode(this->_nodeAl, this->_head);
		}

		void _deleteRebalance(_NodePtr fix, _NodePtr fixParent)
		{
			while (fix != this->_root() && fix->_color == _Node::BLACK) {
				if (fixParent->_left == fix) {
					_NodePtr sibling = fixParent->_right;
					// Case 1: Sibling node of fix is a red node
					if (sibling->_color == _Node::RED) {
						// Transform to case 2
						std::swap(fixParent->_color, sibling->_color);
						this->_leftRotate(fixParent);
						sibling = fixParent->_right;
					}
					// Case 2: Black sibling with two black children
					if (sibling->_left->_color == _Node::BLACK &&
						sibling->_right->_color == _Node::BLACK) {
						// Pop up the BLACK property
						sibling->_color = _Node::RED;
						fix = fixParent;
						fixParent = fix->_parent;
					}
					else {
						// Case 3: Black sibling with red left child and 
						//		   black right child
						if (sibling->_right->_color == _Node::BLACK) {
							// Transform to case 4
							std::swap(sibling->_color, sibling->_left->_color);
							this->_rightRotate(sibling);
							sibling = fixParent->_right;
						}
						// Case 4: Black sibling with red right child
						std::swap(fixParent->_color, sibling->_color);
						sibling->_right->_color = _Node::BLACK;
						this->_leftRotate(fixParent);
						fix = this->_root();
					}
				}
				else {
					// Symestry situation
					_NodePtr sibling = fixParent->_left;
					// Case 1: Sibling node of fix is a red node
					if (sibling->_color == _Node::RED) {
						// Transform to case 2
						std::swap(fixParent->_color, sibling->_color);
						this->_rightRotate(fixParent);
						sibling = fixParent->_left;
					}
					// Case 2: Black sibling with two black children
					if (sibling->_left->_color == _Node::BLACK &&
						sibling->_right->_color == _Node::BLACK) {
						// Pop up the BLACK property
						sibling->_color = _Node::RED;
						fix = fixParent;
						fixParent = fix->_parent;
					}
					else {
						// Case 3: Black sibling with red right child and 
						//		   black left child
						if (sibling->_left->_color == _Node::BLACK) {
							// Transform to case 4
							std::swap(sibling->_color, sibling->_right->_color);
							this->_rightRotate(sibling);
							sibling = fixParent->_left;
						}
						// Case 4: Black sibling with red right child
						std::swap(fixParent->_color, sibling->_color);
						sibling->_left->_color = _Node::BLACK;
						this->_rightRotate(fixParent);
						fix = this->_root();
					}
				}
			}
			fix->_color = _Node::BLACK;
		}

		_NodePtr _auxErase(_NodePtr pos)
		{
			// Record ret ptr for return
			Iterator where(pos);
			_NodePtr ret = (++where)._cur;
			// We need to record fix's parent (after deletetion) rather than 
			// using fix->_parent instead because fix may be nil.
			_NodePtr fix, fixParent;

			// Delete pos
			if (pos->_left->_isNil) {
				fix = pos->_right;
				fixParent = pos->_parent;
				this->_transplant(pos, pos->_right);
			}
			else if (pos->_right->_isNil) {
				fix = pos->_left;
				fixParent = pos->_parent;
				this->_transplant(pos, pos->_left);
			}
			else {
				// Two kids
				_NodePtr nPos = ret;

				if (nPos == pos->_right) {
					fix = nPos;
					fixParent = pos->_parent;
				}
				else {
					fix = nPos->_right;
					fixParent = nPos->_parent;
					this->_transplant(nPos, fix);
					nPos->_right = pos->_right;
					pos->_right->_parent = nPos;
				}
				nPos->_left = pos->_left;
				pos->_left->_parent = nPos;
				this->_transplant(pos, nPos);
				std::swap(nPos->_color, pos->_color);
			}

			if (pos->_color == _Node::BLACK) {
				this->_deleteRebalance(fix, fixParent);
			}

			// Destroy deleted node
			_Node::destroyNode(this->_dataAl, pos);
			_Node::freeNode(this->_nodeAl, pos);
			--this->_size;

			// Reset lef most and right most
			this->_leftMost() = this->_min(this->_root());
			this->_rightMost() = this->_max(this->_root());

			return ret;
		}

		_NodePtr _auxFind(const KeyType &key) const
		{
			_NodePtr pos = _auxLower(key);
			return (pos == this->_head || _comp(key, _getKeyFromNode(pos)) ?
					this->_head : pos);
		}

		_NodePtr _auxLower(const KeyType &key) const
		{
			_NodePtr beg = this->_head;
			_NodePtr tryNode = this->_root();

			// Go left to find key
			while (!tryNode->_isNil) {
				if (_comp(_getKeyFromNode(tryNode), key)) {
					tryNode = tryNode->_right;
				}
				else {
					beg = tryNode;
					tryNode = tryNode->_left;
				}
			}

			return beg;
		}

		_NodePtr _auxUpper(const KeyType &key) const
		{
			_NodePtr end = this->_head;
			_NodePtr tryNode = this->_root();

			while (!tryNode->_isNil) {
				if (_comp(key, _getKeyFromNode(tryNode))) {
					end = tryNode;
					tryNode = tryNode->_left;
				}
				else {
					tryNode = tryNode->_right;
				}
			}

			return end;
		}

	protected:
		KeyCompare _comp;
		AllocatorType _dataAl;
		_NodeAlloc _nodeAl;
	};

}