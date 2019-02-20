#pragma once

// PriorityQueue adaptor standard header

// Mini-STL header
//#include <Config/Config.h>
#include <Alloc/Allocator.h>
#include <Iterator/Iterator.h>
#include <Container/Vector.h>

// STL header and cpp standard header
#include <functional>

namespace MSTD {

	template<
		typename T,
		typename Container = Vector<T>,
		typename Comp = std::less<typename Container::ValueType> 
	>
	class PriorityQueue
	{
	public:
		using ContainerType = Container;
		using SizeType = typename Container::SizeType;
		using Reference = typename Container::Reference;
		using ValueType = typename Container::ValueType;
		using ConstReference = typename Container::ConstReference;

		using _Diff = typename Container::DifferenceType;

		PriorityQueue() :
			PriorityQueue(Comp(), Container())
		{}

		PriorityQueue(const Comp &comp) :
			PriorityQueue(comp, Container())
		{}

		PriorityQueue(const Comp &comp, const Container &con) :
			_heap(con),
			_comp(comp)
		{
			_makeHeap();
		}

		PriorityQueue(const Comp &comp, Container &&con) :
			_heap(move(con)),
			_comp(comp)
		{
			_makeHeap();
		}

		PriorityQueue(const PriorityQueue &that) :
			_heap(that._heap),
			_comp(move(that._comp))
		{}

		PriorityQueue(PriorityQueue &&that) noexcept:
			_heap(move(that._heap)),
			_comp(move(that._comp))
		{}

		template<typename Alloc>
		explicit PriorityQueue(const Alloc &alloc) :
			_heap(alloc),
			_comp()
		{}

		template<typename Alloc>
		explicit PriorityQueue(const Comp &comp, const Alloc &alloc) :
			_heap(alloc),
			_comp(comp)
		{}

		template<typename Alloc>
		explicit PriorityQueue(const Comp &comp, const Container &con, const Alloc &alloc) :
			_heap(con, alloc),
			_comp(comp)
		{}

		template<typename Alloc>
		explicit PriorityQueue(const Comp &comp, Container &&con, const Alloc &alloc) :
			_heap(move(con), alloc),
			_comp(comp)
		{}

		template<typename Alloc>
		explicit PriorityQueue(const PriorityQueue &that, const Alloc &alloc) :
			_heap(that._heap, alloc),
			_comp(that._comp)
		{}

		template<typename Alloc>
		explicit PriorityQueue(PriorityQueue &&that, const Alloc &alloc) :
			_heap(move(that._heap), alloc),
			_comp(move(that._comp))
		{}

		PriorityQueue& operator=(const PriorityQueue &that)
		{
			if (this != addressof(that)) {
				_heap = that._heap;
				_comp = that._comp;
			}
			return *this;
		}

		PriorityQueue& operator=(PriorityQueue &&that) noexcept
		{
			if (this != addressof(that)) {
				_heap = move(that._heap);
				_comp = move(that._comp);
			}
			return *this;
		}

		ConstReference top() const
		{
			return _heap.front();
		}

		bool empty() const
		{
			return _heap.empty();
		}

		SizeType size() const
		{
			return _heap.size();
		}

		void push(const ValueType &val)
		{
			_heap.pushBack(val);
			_precolateUp(_heap.size() - 1);
		}

		void push(ValueType &&val)
		{
			_heap.pushBack(move(val));
			_precolateUp(_heap.size() - 1);
		}

		template< class... Args >
		void emplace(Args&&... args)
		{
			_heap.emplace(forward<Args>(args)...);
			_precolateUp(_heap.size() - 1);
		}

		void pop()
		{
			_heap[0] = _heap.back();
			_heap.popBack();
			_precolateDown(0);
		}

		void swap(PriorityQueue &that) noexcept
		{		
			swap(_heap, that._heap);
			swap(_comp, that._comp);
		}

	protected:
		Container _heap;
		Comp _comp;

	private:
		// Precolate up the element in the end to fit the 
		// property of heap		
		void _precolateUp(SizeType cur)
		{			
			auto key = _heap[cur];
			auto parent = (cur - 1) / 2;
			// Precolate up
			while (cur > 0 && _comp(key, _heap[parent])) {
				_heap[cur] = _heap[parent];
				cur = parent;
				parent = (cur - 1) / 2;
			}
			// Insert key
			_heap[cur] = key;
		}

		// Precolate down the element in the end to fit the 
		// property of heap
		void _precolateDown(SizeType cur)
		{			
			auto key = _heap[cur];
			while (cur < _heap.size()) {
				// Find the most competitive element
				// among cur hole and its children
				SizeType compest = 2 * cur + 1;
				if (compest < _heap.size()) {
					auto right = compest + 1;
					if (right < _heap.size() && _comp(_heap[right], _heap[compest])) {
						compest = right;
					}
					if (_comp(key, _heap[compest])) {
						compest = cur;
					}
				}

				if (compest < _heap.size() && compest != cur) {
					// Move cur pointer to the most 
					// competitive position 					
					_heap[cur] = _heap[compest];
					cur = compest;
				}
				else {
					break;
				}
			}
			// Insert key
			_heap[cur] = key;
		}

		// Build a heap from range [first, last)		
		void _makeHeap()
		{
			if (_heap.size() < 2) {
				return;
			}
			for (SizeType i = (_heap.size() - 1) / 2; i != 0; --i) {
				_precolateDown(i);
			}
			_precolateDown(0);
		}
	};

	template< class T, class Container, class Compare >
	void swap(PriorityQueue<T, Container, Compare>& lhs,
				PriorityQueue<T, Container, Compare>& rhs) noexcept
	{
		lhs.swap(rhs);
	}

}