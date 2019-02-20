#pragma once

/*
 *	Basic memory allocator
 *	with two different allocation 
 *	strategy
 *	Internal use
*/
#include <new>
#include <cstdlib>

namespace MSTD {

	// ================= _DirectMalloc ================

	using AllocHandle = void(*)();

	// A direct memory allocator
	// with bad_alloc handle
	template<int hint>
	class _DirectMalloc {

		static AllocHandle _allocHandle;

		// om : out of memory
		static void* omHandle(size_t n);

		static AllocHandle setAllocHandle(AllocHandle pHandle);

	public:
		// Allocate {n} bytes for client
		static void* alloc(size_t n)
		{
			auto ret = malloc(n);
			if (!ret) {
				return omHandle(n);
			}

			return ret;
		}

		// Deallocate {p} for client
		static void dealloc(void *p, size_t)
		{
			free(p);
		}
	};

	template<int hint>
	AllocHandle _DirectMalloc<hint>::_allocHandle = nullptr;

	template<int hint>
	void* _DirectMalloc<hint>::omHandle(size_t n)
	{
		while (true) {
			if (_allocHandle) {
				_allocHandle();
				// Try allocate again
				auto ret = malloc(n);
				if (ret) {
					return ret;
				}
			}
			else {
				throw std::bad_alloc();
			}
		}
	}

	template<int hint>
	AllocHandle _DirectMalloc<hint>::setAllocHandle(AllocHandle pHandle)
	{
		auto oldHandle = _allocHandle;
		_allocHandle = pHandle;
		return oldHandle;
	}

	// Explicit instance
	using baseAlloc = _DirectMalloc<0>;

	// ================= _DefaultMalloc =================

	// Definition of memory block
	union _Block
	{
		_Block *pNext;
		void *used;
	};

	// Sub allocator
	// Use free list to manage memory pool
	template<int hint>
	class _DefaultMalloc {

		// Minimun block size in memory pool
		static constexpr size_t _BLOCK_SIZE = 8;

		// Number of slots 
		static constexpr size_t _TABLE_SIZE = 16;

		// Threshhold of block size.
		// if memory request exceeds this limit,
		// allocator will delegate allocation task to
		// baseAlloc
		static constexpr size_t _MAX_ALLOC = 128;

		// Free list table
		static _Block *_freeList[_TABLE_SIZE];

		// Total size of allocated heap
		static size_t _heapSize;

		// Start of available memory
		static char* _freeStart;

		// End of available memory
		static char* _freeEnd;

		static size_t roundUp(size_t n) noexcept
		{
			return (n + _BLOCK_SIZE - 1) & ~(_BLOCK_SIZE - 1);
		}

		static size_t index(size_t n) noexcept
		{
			return (n + _BLOCK_SIZE - 1) / _BLOCK_SIZE - 1;
		}

		// Generate new blocks that can contain n bytes 
		// at least assume that {n} has been rounded up to 
		// power of 2.
		// Return the pointer of first free block to client
		static void* refill(size_t n);

		// Request for {nBlock} memory blocks of size {n}
		// it's assumed that {n} has been rounded up to 
		// power of 2.
		// Return the pointer of first free block
		static void* fetchMemory(size_t n, size_t &nBlock);

	public:
		static void* alloc(size_t n)
		{
			if (n > _MAX_ALLOC || n == 0) {
				return baseAlloc::alloc(n);
			}

			auto pBlock = _freeList[index(n)];
			if (!pBlock) { // blocks of size {n} have been used up
				return refill(roundUp(n));
			}

			// Pick out first block of this slot
			_freeList[index(n)] = pBlock->pNext;
			return static_cast<void*>(pBlock);
		}

		static void dealloc(void *p, size_t n)
		{
			if (n > _MAX_ALLOC || n == 0) {
				baseAlloc::dealloc(p, n);
				return;
			}

			// Put back into memory pool.
			// Insert it into the head 
			// of queue
			if (p) {
				auto pBlock = static_cast<_Block *>(p);
				pBlock->pNext = _freeList[index(n)];
				_freeList[index(n)] = pBlock;
			}
		}
	};

	// Initialize every slot to null
	template<int hint>
	_Block* _DefaultMalloc<hint>::_freeList[_TABLE_SIZE] = {
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr
	};

	template<int hint>
	size_t _DefaultMalloc<hint>::_heapSize = 0;

	template<int hint>
	char* _DefaultMalloc<hint>::_freeStart = nullptr;

	template<int hint>
	char* _DefaultMalloc<hint>::_freeEnd = nullptr;

	template<int hint>
	void* _DefaultMalloc<hint>::refill(size_t n)
	{
		//Default request 20 blocks
		size_t nBlock = 20;

		auto pFree = fetchMemory(n, nBlock);

		// Pick out the first block 
		// and return it back to
		// client

		// Construct free Block list
		char *beg = static_cast<char*>(pFree);
		_freeList[index(n)] = nBlock == 1 ? 
			nullptr : static_cast<_Block*>(static_cast<void*>(beg + n));
		// Start from i = 1 since the first block is 
		// goning to be returned to client
		for (size_t i = 1; i < nBlock; ++i) {
			auto current = static_cast<_Block*>(static_cast<void*>(beg + i * n));
			if (i != nBlock - 1) {
				current->pNext = static_cast<_Block*>(static_cast<void*>(beg + (i + 1) * n));
			}
			else {
				current->pNext = nullptr;
			}
		}

		return pFree;
	}

	template<int hint>
	void* _DefaultMalloc<hint>::fetchMemory(size_t n, size_t & nBlock)
	{
		auto memoryLeft = _freeEnd - _freeStart;

		// Maximum number of blocks can be allocated
		auto maxBlock = memoryLeft / n;

		// No more than client's request
		auto actualBlock = maxBlock >= nBlock ? nBlock : maxBlock;

		if (actualBlock > 0) {
			// Alloc success
			void *ret = _freeStart;
			_freeStart += n * actualBlock;
			// Tell clients how many blocks we 
			// give them actually.
			nBlock = actualBlock;

			return ret;
		}
		
		// No more free heap space, so we 
		// have to apply for it.

		// Before apply for it, we should 
		// make use of rest free memory.
		// This is because we will have a
		// new heap space.
		if (memoryLeft > 0) {
			auto newBlock = static_cast<_Block*>(static_cast<void*>(_freeStart));
			newBlock->pNext = _freeList[index(memoryLeft)];
			_freeList[index(memoryLeft)] = newBlock;
		}

		// Apply for new space
		auto memoryRequest = 2 * (n * nBlock) + roundUp(_heapSize >> 4);
		_freeStart = static_cast<char*>(malloc(memoryRequest));

		// Once failed, we have to find some 
		// spare space somewhere else
		if (!_freeStart) {
			// No available free memory in system.
			// We need to solve it by ourself
			// Solving it by borrowing blocks from 
			// other larger free slot
			for (size_t sz = n + _BLOCK_SIZE; sz <= _MAX_ALLOC; sz += _BLOCK_SIZE) {
				auto pb = _freeList[index(sz)];
				if (pb) {
					// Success, we will restart process
					_freeList[index(sz)] = pb->pNext;
					_freeStart = static_cast<char*>(static_cast<void*>(pb));
					_freeEnd = _freeStart + sz;
					return fetchMemory(n, nBlock);
				}
			}

			// Oops! No more internal space.
			// Call base allocator to wait 
			// it useing alloc handle to
			// get some memory.
			_freeEnd = nullptr;
			_freeStart = static_cast<char*>(baseAlloc::alloc(memoryRequest));
		}

		// Finally we got new free memory
		_heapSize += memoryRequest;
		_freeEnd = _freeStart + memoryRequest;
		// Restart process to fetch memory
		return fetchMemory(n, nBlock);
	}

	using defaultAlloc = _DefaultMalloc<0>;
}