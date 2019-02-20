#pragma once

// Internal use
// Base tree standard header

// Mini-STL header

namespace MSTD {

	// Base tree template class
	// to provide some basic operations
	template<typename _ConfigParam, typename _Node>
	class _BaseTree
	{
	public:
		using _SizeType = typename _ConfigParam::SizeType;
		using _NodePtr = _Node*;

		_BaseTree() :
			_head(),
			_size(0)
		{}

	protected:
		_NodePtr _NIL()
		{
			return _head;
		}

		// Tree node operations
		_NodePtr _min(_NodePtr pos) const
		{
			_NodePtr pNode = pos;
			while (!pNode->_isNil && !pNode->_left->_isNil) {
				pNode = pNode->_left;
			}
			return pNode;
		}

		_NodePtr _max(_NodePtr pos) const
		{
			_NodePtr pNode = pos;
			while (!pNode->_isNil && !pNode->_right->_isNil) {
				pNode = pNode->_right;
			}
			return pNode;
		}

		_NodePtr& _root() const
		{
			return _head->_parent;
		}

		_NodePtr& _leftMost() const
		{
			return _head->_left;
		}

		_NodePtr& _rightMost() const
		{
			return _head->_right;
		}

		// Single rotation (left and right)
		// Left
		void _leftRotate(_NodePtr ptr)
		{
			if (!ptr->_right->_isNil) {
				// New root
				auto nRoot = ptr->_right;
				// adjust child node
				ptr->_right = nRoot->_left;
				if (!nRoot->_left->_isNil) {
					nRoot->_left->_parent = ptr;
				}
				nRoot->_left = ptr;

				// Set parent's pointer
				_transplant(ptr, nRoot);
				ptr->_parent = nRoot;
			}
		}
		// Right
		void _rightRotate(_NodePtr ptr)
		{
			if (!ptr->_left->_isNil) {
				auto nRoot = ptr->_left;

				// adjust chuild
				ptr->_left = nRoot->_right;
				if (!nRoot->_right->_isNil) {
					nRoot->_right->_parent = ptr;
				}
				nRoot->_right = ptr;

				// Set parent's pointer
				_transplant(ptr, nRoot);
				ptr->_parent = nRoot;
			}
		}

		// Substitude node x with y for parent
		// of node x.
		// This fucntion may be used when 
		// we're going to extract some node x
		// from the tree.
		void _transplant(_NodePtr x, _NodePtr y)
		{
			if (x->_parent->_isNil) {
				_root() = y;
			}
			else if (x->_parent->_left == x) {
				x->_parent->_left = y;
			}
			else {
				x->_parent->_right = y;
			}
			if (!y->_isNil) {
				y->_parent = x->_parent;
			}
		}

		// Insert a new node before ({addLeft}) or after {pos}
		void _insertAt(_NodePtr pNode, _NodePtr pos, bool addLeft)
		{
			pNode->_left = _NIL();
			pNode->_right = _NIL();
			pNode->_parent = pos;
			if (pos->_isNil) {
				_root() = pNode;
				_leftMost() = pNode;
				_rightMost() = pNode;
			}
			else if (addLeft) {
				pos->_left = pNode;
				if (pos == _leftMost()) {
					_leftMost() = pNode;
				}
			}
			else {
				pos->_right = pNode;
				if (pos == _rightMost()) {
					_rightMost() = pNode;
				}
			}
			++_size;
		}

	protected:
		_NodePtr _head;
		_SizeType _size;
	};

}