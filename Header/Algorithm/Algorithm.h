#pragma once

#include <Iterator/Iterator.h>
#include <Alloc/Allocator.h>

#include <initializer_list>
#include <functional>
#include <cstring>
#include <algorithm>

// Algorithms standard header

namespace MSTD {

	/**
	 * Compare the elements of [first1, last1) and [first2, first2 + (last1 - first1)],
	 * no optimization
	 *
	 * @param first1 - Start position of range
	 * @param last1	- End position of range
	 * @param first2 - Start position of range
	 * @param op - Compare operation
	 *
	 * @return
	 *	If all the compared elements are equal, then return true;
	 *	Otherwise, return false;
	 */
	template<typename InputIt1, typename InputIt2, typename CompOp = std::equal_to<>>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, CompOp op = CompOp())
	{
		for (; first1 != last1; ++first1, ++first2) {
			if (!op(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

	/**
	 *  Assigns the given value to the elements in the range [first, last),
	 *  no optimization.
	 *  
	 * @param first - Start position of range
	 * @param last	- End position of range
	 * @param val	- The value to be assigned
	 */
	template<typename OutputIt, typename T>
	void fill(OutputIt first, OutputIt last, const T &val)
	{
		for (; first != last; ++first) {
			*first = val;
		}
	}

	/**
	 * Assigns the given value to the first count elements in the range beginning 
	 * at first if count > 0. Does nothing otherwise, no optimization.
	 * 
	 * @param first - Start position of range	 
	 * @param n - The number of value to be assigned
	 * @param val - The value to be assigned
	 * 
	 * @return
	 *	Iterator one past the last element assigned if count > 0, first otherwise.
	 */
	template<typename OutputIt, typename T>
	OutputIt fill_N(OutputIt first, size_t n, const T &val)
	{
		for (; n > 0; --n, ++first) {
			*first = val;
		}
		return first;
	}

	/**
	 * Checks if the first range [first1, last1) is lexicographically less than 
	 * the second range [first2, last2), no optimization.
	 * 
	 * @param first1 - Start position of range
	 * @param last1	- End position of range	 
	 * @param first2 - Start position of range
	 * @param last2	- End position of range
	 * @param op - Compare operation
	 * 
	 * @return
	 *	true if the first range is lexicographically less than the second. 
	 */
	template<typename InputIt1, typename InputIt2, typename CompareOp = std::less<>>
	bool lexicographicalCompare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2, CompareOp op = CompareOp())
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (op(*first1, *first2)) {
				return true;
			}
			else if (op(*first2, *first1)) {
				return false;
			}
		}
		return first1 == last1 && first2 != last2;
	}

	/**
	 * Select the maximum element of two object, using the given operator
	 * 
	 * @param lhs - values to compare 
	 * @param rhs - values to compare 
	 * @param op - Compare operation
	 * 
	 * @return
	 *	Returns the greater of lhs and rhs. 
	 */
	template<typename T, typename CompareOp = std::less<>>
	const T& max(const T &lhs, const T &rhs, CompareOp op = CompareOp())
	{
		return op(lhs, rhs) ? rhs : lhs;
	}

	template<typename T, typename CompareOp = std::less<>>
	T max(std::initializer_list<T> il, CompareOp op = CompareOp())
	{
		T ret = *il.begin();
		for (auto &&val : il) {
			if (op(ret, val)) {
				ret = val;
			}
		}
		return ret;
	}

	/**
	 * Select the minimum element of two object, using the given operator
	 *
	 * @param lhs - values to compare
	 * @param rhs - values to compare
	 * @param op - Compare operation
	 *
	 * @return
	 *	Returns the less of lhs and rhs.
	 */
	template<typename T, typename CompareOp = std::less<>>
	const T& min(const T &lhs, const T &rhs, CompareOp op = CompareOp())
	{
		return !op(lhs, rhs) ? rhs : lhs;
	}

	template<typename T, typename CompareOp = std::less<>>
	T min(std::initializer_list<T> il, CompareOp op = CompareOp())
	{
		T ret = *il.begin();
		for (auto &&val : il) {
			if (op(val, ret)) {
				ret = val;
			}
		}
		return ret;
	}

	// Copy elements from [first, last) to dest with no optimization
	template<typename InputIt, typename OutputIt>
	OutputIt _auxCopy(InputIt first, InputIt last, OutputIt dest, MSTD::falseType)
	{
		for (; first != last; ++first, ++dest) {
			*dest = *first;
		}
		return dest;
	}

	// Copy elements from [first, last) to dest with optimization
	template<typename InputIt, typename OutputIt>
	OutputIt _auxCopy(InputIt first, InputIt last, OutputIt dest, MSTD::trueType)
	{
		using Value = typename  MSTD::IteratorTraits<InputIt>::ValueType;
		auto count = MSTD::distance(first, last);
		const auto firstChar = static_cast<void*>(first);
		const auto destChar = static_cast<void*>(dest);
		std::memmove(destChar, firstChar, count * sizeof(Value));
		return (dest + count);
	}

	/**
	 * Copy the elements of range [first, last) to dest
	 * 
	 * @param first - Start position of range
	 * @param last	- End position of range
	 * @param dest	- Start of destination range
	 * 
	 * @return
	 *	The past to the last element of [dest, dest + first - last)
	 */
	template<typename InputIt, typename OutputIt>
	OutputIt copy(InputIt first, InputIt last, OutputIt dest)
	{
		using SourceValue = typename MSTD::IteratorTraits<InputIt>::ValueType;
		using DestValue = typename MSTD::IteratorTraits<OutputIt>::ValueType;
		using SourceItType = typename MSTD::IteratorTraits<InputIt>::IteratorCategory;
		using DestItType = typename MSTD::IteratorTraits<OutputIt>::IteratorCategory;

		return _auxCopy(first, last, dest,
					typename MSTD::conditional<
						MSTD::isPointer<SourceItType>::value &&
						MSTD::isPointer<DestItType>::value &&						
						MSTD::isTrviallyAssignable<DestValue, SourceValue>::value,
						MSTD::trueType, MSTD::falseType
					>::type()
				);
	}

	/**
	 * Copy the elements of range [first, last) to dest when 
	 * the unary operator returns true.
	 *
	 * @param first - Start position of range
	 * @param last	- End position of range
	 * @param dest	- Start of destination range
	 *
	 * @return
	 *	The past to the last element of [dest, dest + first - last)
	 */
	template<typename InputIt, typename OutputIt, typename  UnaryOp>
	OutputIt CopyIf(InputIt first, InputIt last, OutputIt dest, UnaryOp op)
	{
		while (first != last) {
			if (op(*first)) {
				*dest++ = *first;
			}
			++first;
		}
		return dest;
	}

	/**
	 * Get the union set of two sorted ranges.
	 * 
	 * @param first1 - Start position of range
	 * @param last1	- End position of range
	 * @param first2 - Start position of range
	 * @param last2	- End position of range
	 * @param dest	- Destination
	 * @param op - Compare operation
	 * 
	 * @return
	 *	The iterator past to the end of sorted union set.
	 */
	template<typename InputIt, typename OutputIt, typename CompareOp = std::less<>>
	OutputIt setUnion(InputIt first1, InputIt last1, 
					  InputIt first2, InputIt last2,
					  OutputIt dest, CompareOp op = CompareOp())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first1, *first2)) {
				*dest++ = *first1++;
			}
			else if (op(*first2, *first1)) {
				*dest++ = *first2++;
			}
			else { // *first1 == *first2
				*dest++ = *first1;
				++first1;
				++first2;
			}
		}

		return copy(first2, last2, copy(first1, last1, dest));
	}

	/**
	* Get the intersection set of two sorted ranges.
	*
	* @param first1 - Start position of range
	* @param last1	- End position of range
	* @param first2 - Start position of range
	* @param last2	- End position of range
	* @param dest	- Destination
	* @param op - Compare operation
	*
	* @return
	*	The iterator past to the end of sorted intersection set.
	*/
	template<typename InputIt, typename OutputIt, typename CompareOp = std::less<>>
	OutputIt setIntersection(InputIt first1, InputIt last1,
							 InputIt first2, InputIt last2,
							 OutputIt dest, CompareOp op = CompareOp())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first1, *first2)) {
				++first1;
			}
			else if (op(*first2, *first1)) {
				++first2;
			}
			else { // *first1 == *first2
				*dest++ = *first1;
				++first1;
				++first2;
			}
		}

		return dest;
	}

	/**
	* Get the difference set of two sorted ranges.
	*
	* @param first1 - Start position of range
	* @param last1	- End position of range
	* @param first2 - Start position of range
	* @param last2	- End position of range
	* @param dest	- Destination
	* @param op - Compare operation
	*
	* @return
	*	The iterator past to the end of sorted difference set.
	*/
	template<typename InputIt, typename OutputIt, typename CompareOp = std::less<>>
	OutputIt setDifference(InputIt first1, InputIt last1,
						   InputIt first2, InputIt last2,
						   OutputIt dest, CompareOp op = CompareOp())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first1, *first2)) {
				*dest++ = *first1++;
			}
			else if (op(*first2, *first1)) {
				++first2;
			}
			else { // *first1 == *first2s
				++first1;
				++first2;
			}
		}

		return copy(first1, last1, dest);
	}

	/**
	* Get the difference set of two sorted ranges.
	*
	* @param first1 - Start position of range
	* @param last1	- End position of range
	* @param first2 - Start position of range
	* @param last2	- End position of range
	* @param dest	- Destination
	* @param op - Compare operation
	*
	* @return
	*	The iterator past to the end of sorted difference set.
	*/
	template<typename InputIt, typename OutputIt, typename CompareOp = std::less<>>
	OutputIt setSymmetricDifference(InputIt first1, InputIt last1,
								   InputIt first2, InputIt last2,
								   OutputIt dest, CompareOp op = CompareOp())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first1, *first2)) {
				*dest++ = *first1++;
			}
			else if (op(*first2, *first1)) {
				*dest++ = *first2++;
			}
			else { // *first1 == *first2s
				++first1;
				++first2;
			}
		}

		return copy(first2, last2, copy(first1, last1, dest));
	}

	// heap operations

	// Shift up the last - 1 element to a proper position in a heap.
	template<typename RandomIt, typename Compare>
	void _auxShiftUp(RandomIt first, RandomIt last, Compare op)
	{
		using _Diff = typename IteratorTraits<RandomIt>::DifferenceType;
		using _Value = typename IteratorTraits<RandomIt>::ValueType;
		
		_Diff len = last - first;
		_Diff holeIdx = len - 1;
		_Value key = MSTD::move(*(first + holeIdx));

		while (holeIdx > 0) {
			_Diff parentIdx = (holeIdx - 1) / 2;
			if (op(*(first + parentIdx), key)) {
				// Shift up element
				*(first + holeIdx) = *(first + parentIdx);
				holeIdx = parentIdx;
			}
			else {
				break;
			}
		}
		// Put key in the final hole
		*(first + holeIdx) = MSTD::move(key);
	}

	// Shift down the cur element to a proper position
	template<typename RandomIt, typename Compare>
	void _auxShiftDown(RandomIt first, RandomIt last, RandomIt cur, Compare op)
	{
		using _Diff = typename IteratorTraits<RandomIt>::DifferenceType;
		using _Value = typename IteratorTraits<RandomIt>::ValueType;

		_Diff len = last - first;
		_Diff holeIdx = cur - first;

		while (holeIdx < len) {
			// Find largest child
			_Diff nextIdx = holeIdx;
			_Diff leftIdx = 2 * holeIdx + 1;
			_Diff rightIdx = 2 * holeIdx + 2;

			if (leftIdx < len && op(*(first + nextIdx), *(first + leftIdx))) {
				// Go to left
				nextIdx = leftIdx;
			}
			if (rightIdx < len && op(*(first + nextIdx), *(first + rightIdx))) {
				// Go to right
				nextIdx = rightIdx;
			}

			if (nextIdx == holeIdx) {
				break;
			}
		   	// Shift down
			_Value tmp = MSTD::move(*(first + holeIdx));
			*(first + holeIdx) = MSTD::move(*(first + nextIdx));
			*(first + nextIdx) = MSTD::move(tmp);
			// Go down
			holeIdx = nextIdx;
		}
	}

	template<typename RandomIt, typename Compare = std::less<>>
	void pushHeap(RandomIt first, RandomIt last, Compare op = Compare())
	{
		_auxShiftUp(first, last, op);
	}

	template<typename RandomIt, typename Compare = std::less<>>
	void popHeap(RandomIt first, RandomIt last, Compare op)
	{
		if (last - first > 1) {
			using std::swap;
			swap(*first, *--last);
			_auxShiftDown(first, last, first, op);
		}
	}

	template<typename RandomIt, typename Compare = std::less<>>
	void makeHeap(RandomIt first, RandomIt last, Compare op = Compare())
	{
		using _Diff = typename IteratorTraits<RandomIt>::DifferenceType;
		_Diff len = last - first;
		for (_Diff curIdx = (len - 2) / 2; curIdx >= 0; --curIdx) {
			_auxShiftDown(first, last, first + curIdx, op);
		}
	}

	template<typename RandomIt, typename Compare = std::less<>>
	void sortHeap(RandomIt first, RandomIt last, Compare op = Compare())
	{
		for (; last - first > 1; --last) {
			popHeap(first, last, op);
		}
	}

	template<typename RandomIt, typename Compare = std::less<>>
	RandomIt _auxIsHeapUtil(RandomIt first, RandomIt last, Compare op)
	{
		using _Diff = typename IteratorTraits<RandomIt>::DifferenceType;

		_Diff len = last - first;
		_Diff maxNonLeaf = (len - 2) / 2;
		_Diff cur = 0;

		while (cur <= maxNonLeaf) {
			_Diff leftIdx = 2 * cur + 1;
			_Diff rightIdx = 2 * cur + 2;
			if (op(*(first + cur), *(first + leftIdx))) {
				return (first + leftIdx);
			}
			if (rightIdx >= len || op(*(first + cur), *(first + rightIdx))) {
				return (first + rightIdx);
			}
			++cur;
		}
		return last;
	}

	template<typename RandomIt, typename Compare = std::less<>>
	RandomIt isHeapUtil(RandomIt first, RandomIt last, Compare op = Compare())
	{
		return _auxIsHeapUtil(first, last, op);
	}

	template<typename RandomIt, typename Compare = std::less<>>
	bool isHeap(RandomIt first, RandomIt last, Compare op = Compare())
	{
		return (_auxIsHeapUtil(first, last, op) == last);
	}

	template<typename ForwardIt, typename Compare = std::equal_to<>>
	ForwardIt adjacentFind(ForwardIt first, ForwardIt last, Compare op = Compare())
	{
		if (first == last) {
			return last;
		}
		ForwardIt next = first;
		while (++next != last) {
			if (op(*first, *next)) {
				return first;
			}
			first = next;
		}
		return last;
	}

	// Sequence operations

	template<typename InputIt, typename T>
	auto count(InputIt first, InputIt last, const T &val)
		-> typename MSTD::IteratorTraits<InputIt>::DifferenceType
	{
		using _Diff = typename MSTD::IteratorTraits<InputIt>::DifferenceType;
		_Diff count = 0;
		while (first != last) {
			if (*first == val) {
				++count;
			}
		}
		return count;
	}

	template<typename InputIt, typename UnaryOp>
	auto countIf(InputIt first, InputIt last, UnaryOp op) 
		-> typename MSTD::IteratorTraits<InputIt>::DifferenceType
	{
		using _Diff = typename MSTD::IteratorTraits<InputIt>::DifferenceType;
		_Diff count = 0;
		while (first != last) {
			if (op(*first)) {
				++count;
			}
		}
		return count;
	}

	template<typename InputIt, typename T>
	InputIt find(InputIt first, InputIt last, const T &val)
	{
		while (first != last) {
			if (*first == val) {
				return first;
			}
		}
		return last;
	}

	template<typename InputIt, typename UnaryOp>
	InputIt findIf(InputIt first, InputIt last, UnaryOp op)
	{
		while (first != last) {
			if (op(*first)) {
				return first;
			}
		}
		return last;
	}

	template<typename InputIt, typename ForwardIt, typename Binary = std::equal_to<>>
	InputIt findFirstOf(InputIt first, InputIt last, 
						ForwardIt sFirst, ForwardIt sLast, Binary op = Binary())
	{
		for (; first != last; ++first) {
			for (ForwardIt it = sFirst; it != sLast; ++it) {
				if (op(*it, *first)) {
					return first;
				}
			}
		}
		return last;
	}

	template<typename InputIt, typename UnaryFunc>
	UnaryFunc forEach(InputIt first, InputIt last, UnaryFunc func)
	{
		for (; first != last; ++first) {
			func(*first);
		}
		return func;
	}

	template<typename ForwardIt, typename GenFunc>
	void generate(ForwardIt first, ForwardIt last, GenFunc gen)
	{
		for (; first != last; ++first) {
			*first = gen();
		}
	}

	template<typename ForwardIt, typename GenFunc>
	void generate_N(ForwardIt first, ForwardIt last, size_t n, GenFunc gen)
	{
		for (; n > 0; ++first, --n) {
			*first = gen();
		}
	}

	template<typename InputIt, typename Compare = std::less<>>
	bool includes(InputIt first1, InputIt last1,
				  InputIt first2, InputIt last2, Compare op = Compare())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first2, *first1)) {
				return false;
			}
			else if (op(*first1, *first2)) {
				++first1;
			}
			else {
				++first1;
				++first2;
			}
		}

		return first2 == last2;
	}

	template<typename ForwardIt, typename Compare = std::less<>>
	ForwardIt maxElement(ForwardIt first, ForwardIt last, Compare op = Compare())
	{
		if (first == last) {
			return last;
		}
		ForwardIt ret = first;
		while (++first != last) {
			if (op(*ret, *first)) {
				ret = first;
			}
		}
		return ret;
	}

	template<typename ForwardIt, typename Compare = std::less<>>
	ForwardIt minElement(ForwardIt first, ForwardIt last, Compare op = Compare())
	{
		if (first == last) {
			return last;
		}
		ForwardIt ret = first;
		while (++first != last) {
			if (op(*first, *ret)) {
				ret = first;
			}
		}
		return ret;
	}

	template<typename InputIt, typename OutputIt, typename Compare = std::less<>>
	OutputIt merge(InputIt first1, InputIt last1,
				   InputIt first2, InputIt last2,
				   OutputIt dest, Compare op = Compare())
	{
		while (first1 != last1 && first2 != last2) {
			if (op(*first2, *first1)) {
				*dest++ = *first2++;
			}
			else {
				*dest++ = *first1++;
			}
		}

		return copy(first2, last2, copy(first1, last1, dest));
	}

	template<typename BidirectionalIt, typename Unary>
	BidirectionalIt partition(BidirectionalIt first, BidirectionalIt last, Unary op)
	{
		for (BidirectionalIt next = first; next != last; ++next) {
			if (op(*next)) {
				using std::swap;
				swap(*first, *next);
				++first;
			}
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt remove(ForwardIt first, ForwardIt last, const T &val)
	{
		first = find(first, last, val);
		ForwardIt next = first;

		if (first != last) {
			while (++first != last) {
				if (!(val == *first)) {
					*next++ = MSTD::move(*first);
				}
			}
		}
		return next;
	}

	template<typename ForwardIt, typename OutputIt, typename T>
	OutputIt removeCopy(ForwardIt first, ForwardIt last, OutputIt dest, const T &val)
	{		
		while (first != last) {
			if (!(val == *first)) {
				*dest++ = *first;
			}
			++first;
		}
		return dest;
	}

	template<typename ForwardIt, typename Unary>
	ForwardIt removeIf(ForwardIt first, ForwardIt last, Unary op)
	{
		first = findIf(first, last, op);
		ForwardIt next = first;
		if (first != last) {
			while (++first != last) {
				if (!op(*first)) {
					*next++ = MSTD::move(*first);
				}
			}
		}
		return next;
	}

	template<typename ForwardIt, typename OutputIt, typename Unary>
	OutputIt removeCopyIf(ForwardIt first, ForwardIt last, OutputIt dest, Unary op)
	{
		while (first != last) {
			if (!op(*first)) {
				*dest++ = *first;
			}
			++first;
		}
		return dest;
	}

	template<typename ForwardIt, typename T>
	void replace(ForwardIt first, ForwardIt last, const T &val)
	{
		while (first != last) {
			if (*first == val) {
				*first = val;
			}
			++first;
		}
	}

	template<typename ForwardIt, typename T, typename Unary>
	void replaceIf(ForwardIt first, ForwardIt last, Unary op, const T &val)
	{
		while (first != last) {
			if (op(*first)) {
				*first = val;
			}
			++first;
		}
	}

	template<typename ForwardIt, typename OutputIt, typename T>
	OutputIt replaceCopy(ForwardIt first, ForwardIt last, OutputIt dest, const T &val)
	{
		while (first != last) {
			*dest++ = (*first == val) ? val : *first;
			++first;
		}
		return dest;
	}

	template<typename ForwardIt, typename OutputIt, typename Unary, typename T>
	OutputIt replaceCopyIf(ForwardIt first, ForwardIt last, OutputIt dest, Unary op, const T &val)
	{
		while (first != last) {
			*dest++ = (*first == val) ? val : *first;
			++first;
		}
		return dest;
	}

	template<typename BidirectionalIt>
	void _auxReverse(BidirectionalIt first, BidirectionalIt last, MSTD::BidirectionalIteratorTag)
	{
		while (first != last && first != --last) {
			using std::swap;
			swap(*first++, *last);
		}
	}

	template<typename RandomIt>
	void _auxReverse(RandomIt first, RandomIt last, MSTD::RandomAccessIteratorTag)
	{
		while (first < last) {
			using std::swap;
			swap(*first++, *--last);
		}
	}

	template<typename Iterator>
	void reverse(Iterator first, Iterator last)
	{
		_auxReverse(
			first, 
			last,
			typename MSTD::IteratorTraits<Iterator>::IteratorCategory()
		);
	}

	template<typename BidirectionalIt, typename OutputIt>
	OutputIt reverseCopy(BidirectionalIt first, BidirectionalIt last, OutputIt dest)
	{
		while (first != last) {
			--last;
			*dest++ = *last;
		}
		return dest;
	}

	// Rotation operators, with no optimization

	template<typename ForwardIt>
	ForwardIt _auxRotate(ForwardIt first, ForwardIt mid, ForwardIt last,
						 MSTD::ForwardIteratorTag)
	{
		ForwardIt next = mid;
		ForwardIt ret = last;
		while (true) {
			using std::swap;
			swap(*first++, *next++);
			if (first == mid) { // Reach mid, left interval exhausted
				if (next == last) {
					// Finish rotation
					return ret == last ? mid : ret;
				}
				mid = next;
			}
			else if (next == last) { // Reach last, right interval exhausted
				if (ret == last) {
					ret = first;
				}
				next = mid;
			}
		}
	}

	template<typename ForwardIt>
	ForwardIt _auxRotate(ForwardIt first, ForwardIt mid, ForwardIt last,
						 MSTD::RandomAccessIteratorTag)
	{
		reverse(first, mid);
		reverse(mid, last);
		reverse(first, last);
		return (first + (last - mid));
	}

	template<typename Iterator>
	Iterator rotate(Iterator first, Iterator mid, Iterator last)
	{
		if (first == mid) {
			return last;
		}
		else if (mid == last) {
			return first;
		}
		return _auxRotate(
			first,
			mid,
			last,
			typename MSTD::IteratorTraits<Iterator>::IteratorCategory()
		);
	}

	template<typename ForwardIt, typename OutputIt>
	OutputIt rotateCopy(ForwardIt first, ForwardIt mid, ForwardIt last, OutputIt dest)
	{
		return copy(first, mid, copy(mid, last, dest));
	}

	template<typename ForwardIt1, typename ForwardIt2, typename Equality = std::equal_to<>>
	ForwardIt1 search(ForwardIt1 first1, ForwardIt1 last1, 
					  ForwardIt2 first2, ForwardIt2 last2, Equality eq = Equality())
	{
		auto d1 = MSTD::distance(first1, last1);
		auto d2 = MSTD::distance(first2, last2);

		if (d1 < d2) {
			return last1;
		}

		auto cur1 = first1;
		auto cur2 = first2;
		while (cur2 != last2) {
			if (eq(*cur1, *cur2)) {
				++cur1;
				++cur2;
			}
			else {
				// Restart search at beginning of [first2, last2)
				if (d1 == d2) {
					return last1;
				}
				// Redirect cur1 and cur2.
				cur1 = ++first1;
				cur2 = first2;
				--d1;
			}
		}
		return first1;
	}

	template<typename ForwardIt1, typename ForwardIt2>
	ForwardIt2 swapRange(ForwardIt1 first1, ForwardIt1 last1,
						 ForwardIt2 first2, ForwardIt2 last2)
	{
		for (; first1 != last2; ++first1, ++first2) {
			using std::swap;
			swap(*first1, *first2);
		}
		return first2;
	}

	template<typename ForwardIt, typename OutputIt, typename UnaryOp>
	OutputIt transform(ForwardIt first, ForwardIt last, OutputIt dest, UnaryOp op)
	{
		for (; first != last; ++first) {
			*dest++ = op(*first);
		}
		return dest;
	}

	template<typename ForwardIt1, typename ForwardIt2, typename OutputIt, typename BinaryOp>
	OutputIt transform(ForwardIt1 first1, ForwardIt1 last1,
					   ForwardIt2 first2, OutputIt dest, BinaryOp op)
	{
		for (; first1 != last1; ++first1, first2) {
			*dest++ = op(*first1, *first2);
		}
			return dest;
	}

	template<typename ForwardIt, typename Equality = std::equal_to<>>
	ForwardIt unique(ForwardIt first, ForwardIt last, Equality eq = Equality())
	{	
		if (first == last) {
			return first;
		}
		auto scanStart = first++;
		auto scanEnd = first;

		while (scanEnd != last) {
			if (!eq(*scanStart, *scanEnd)) {
				// Find a new element
				scanStart = scanEnd;
				*first++ = MSTD::move(*scanStart);
			}
			++scanEnd;
		}
		return first;
	}

	template<typename ForwardIt, typename T, typename Comp>
	ForwardIt _auxLowerBound(ForwardIt first, ForwardIt last, 
							 const T &val, Comp comp,
							 MSTD::ForwardIteratorTag)
	{
		auto len = MSTD::distance(first, last);
		ForwardIt ret = last;

		while (len > 0) {
			ForwardIt middle = MSTD::advance(first, len / 2);
			if (!comp(*middle, val)) {
				// Go left
				ret = middle;
				len = len / 2;
			}
			else {
				// Go right
				first = ++middle;
				len = len - len / 2 - 1;
			}
		}
		return ret;
	}

	template<typename ForwardIt, typename T, typename Comp>
	ForwardIt _auxLowerBound(ForwardIt first, ForwardIt last,
							 const T &val, Comp comp,
							 MSTD::RandomAccessIteratorTag)
	{
		auto len = last - first;
		ForwardIt ret = last;

		while (len > 0) {
			ForwardIt middle = first + len / 2;
			if (!comp(*middle, val)) {
				// Go left
				ret = middle;
				len = len / 2;
			}
			else {
				// Go right
				first = ++middle;
				len = len - len / 2 - 1;
			}
		}

		return ret;
	}

	template<typename ForwardIt, typename T, typename Comp = std::less<>>
	ForwardIt lowerBound(ForwardIt first, ForwardIt last, const T &val, Comp comp = Comp())
	{
		return _auxLowerBound(first, last, val, comp, 
			typename MSTD::IteratorTraits<ForwardIt>::IteratorCategory());
	}

	template<typename ForwardIt, typename T, typename Comp>
	ForwardIt _auxUpperBound(ForwardIt first, ForwardIt last,
							 const T &val, Comp comp,
							 MSTD::ForwardIteratorTag)
	{
		auto len = MSTD::distance(first, last);
		ForwardIt ret = last;

		while (len > 0) {
			ForwardIt middle = MSTD::advance(first, len / 2);
			if (comp(val, *middle)) {
				// Go left
				ret = middle;
				len = len / 2;
			}
			else {
				// Go right
				first = ++middle;
				len = len - len / 2 - 1;
			}
		}
		return ret;
	}

	template<typename ForwardIt, typename T, typename Comp>
	ForwardIt _auxUpperBound(ForwardIt first, ForwardIt last,
							 const T &val, Comp comp,
							 MSTD::RandomAccessIteratorTag)
	{
		auto len = last - first;
		ForwardIt ret = last;

		while (len > 0) {
			ForwardIt middle = first + len / 2;
			if (comp(val, *middle)) {
				// Go left
				ret = middle;
				len = len / 2;
			}
			else {
				// Go right
				first = ++middle;
				len = len - len / 2 - 1;
			}
		}

		return ret;
	}

	template<typename ForwardIt, typename T, typename Comp = std::less<>>
	ForwardIt upperBound(ForwardIt first, ForwardIt last, const T &val, Comp comp = Comp())
	{
		return _auxUpperBound(first, last, val, comp,
			typename MSTD::IteratorTraits<ForwardIt>::IteratorCategory());
	}

	template<typename ForwardIt, typename T, typename Comp = std::less<>>
	bool binarySearch(ForwardIt first, ForwardIt last, const T &val, Comp comp = Comp())
	{
		auto it = lowerBound(first, last, val, comp);
		return !(it == last) && !comp(val, *it);
	}

	template<typename BidirIt, typename Comp = std::less<>>
	bool nextPermutation(BidirIt first, BidirIt last, Comp comp = Comp())
	{
		if (first == last) {
			return false;
		}

		BidirIt next = last;
		BidirIt pivot = --next;

		// Find longest non-increasing suffix
		while (pivot != first && !comp(*--pivot, *next)) {
			--next;
		}

		if (pivot == first && !comp(*pivot, *next)) {
			// No larger permutation
			reverse(first, last);
			return false;
		}
		
		BidirIt point = last;
		while (!comp(*pivot, *--point)) {
			continue;
		}

		// Increase this sequence to next permutation
		using std::swap;
		swap(*pivot, *point);
		reverse(next, last);
		return true;
	}

	template<typename BidirIt, typename Comp = std::less<>>
	bool prevPermutation(BidirIt first, BidirIt last, Comp comp = Comp())
	{
		if (first == last) {
			return false;
		}

		BidirIt next = last;
		BidirIt pivot = --next;

		// Find longest non-decreasing suffix
		while (pivot != first && !comp(*next, *--pivot)) {
			--next;
		}

		if (pivot == first && !comp(*next, *pivot)) {
			// No smaller permutation
			reverse(first, last);
			return false;
		}
		
		BidirIt point = last;
		while (!comp(*--point, *pivot)) {
			continue;
		}

		// Increase this sequence to next permutation
		using std::swap;
		swap(*pivot, *point);
		reverse(next, last);
		return true;
	}

	// Sort

// Threshold of using insertion sort
#define MAX_INSERT_ELE 32

	// Insertion sort
	template<typename RanIt, typename Compare = std::less<>>
	void _auxInsertionSort(RanIt first, RanIt last, Compare comp = Compare())
	{
		using Value = typename MSTD::IteratorTraits<RanIt>::ValueType;
		if (first != last) {
			for (RanIt next = first; ++next != last; ) {
				Value val = MSTD::move(*next);

				// Optimization for insertion sort
				//if (comp(val, *first)) {
				//	// Move sequence backward directly.
				//	std::copy_backward(first, next, ++next);
				//	// Put val in proper position
				//	*first = MSTD::move(val);
				//}
				//else 
				{
					// Proper insertion
					auto tmp = next - 1;
					for (; comp(val, *tmp); --tmp) {
						*(tmp + 1) = MSTD::move(*tmp);
					}
					*(tmp + 1) = MSTD::move(val);
				}
			}
		}
	}

	// Choosing the median ele among 
	// first, mid and last. 
	// Then put the median ele in last
	template<typename RanIt, typename Compare>
	void _auxMed3(RanIt first, RanIt mid, RanIt last, Compare comp)
	{
		using std::swap;
		if (comp(*first, *mid)) { // first < mid
			if (comp(*mid, *last)) {
				// first < mid < last
				swap(*mid, *last);
			}
			else if (comp(*first, *last)) { // first < last <= mid
				// do nothing
			}
			else {
				swap(*first, *last);
			}
		}
		else {// mid <= first
			if (comp(*first, *last)) { // mid <= first < last
				swap(*first, *last);
			}
			else if (comp(*mid, *last)) { // mid < last <= first
				// do nothing
			}
			else { // last <= mid <= first
				swap(*mid, *last);
			}
		}
	}

	// Partition
	// Use middle-in-three to divide this range
	template<typename RanIt, typename Compare = std::less<>>
	RanIt _auxPartition(RanIt first, RanIt last, Compare comp = Compare())
	{
		if (first == last) {
			return last;
		}
		_auxMed3(first, first + (last - first) / 2, last - 1, comp);
		RanIt pivot = --last;

		--last;
		while (true) {
			using std::swap;
			while (comp(*first, *pivot)) {
				++first;
			}
			while (comp(*pivot, *last)) {
				--last;
			}
			if (first >= last) {
				// Put pivot into a proper position
				swap(*first, *pivot);
				return first;
			}
			swap(*first++, *last--);
		}
	}

	// Use intro sort
	template<typename RanIt, typename Diff, typename Compare = std::less<>>
	void _auxSort(RanIt first, RanIt last, Diff idea, Compare comp = Compare())
	{
		if (first == last) {
			return;
		}

		auto count = last - first;
		// Quick sort
		while (count > MAX_INSERT_ELE && idea > 0) {
			idea >>= 1; // Divide log_2(N) times at most
			RanIt mid = _auxPartition(first, last, comp);
			// Continue to divide longer half
			if (mid - first < last - mid) {
				// Continue to divide second half
				_auxSort(first, mid, idea, comp);
				first = mid;
			}
			else { // Continue ti divide first half
				_auxSort(mid, last, idea, comp);
				last = mid;
			}
		}

		if (last - first > MAX_INSERT_ELE) {
			// heap sort if divided too many times
			MSTD::makeHeap(first, last, comp);
			MSTD::sortHeap(first, last, comp);
		}
		else if (count >= 2) {
			_auxInsertionSort(first, last, comp);
		}
	}

	template<typename RanIt, typename Compare = std::less<>>
	void sort(RanIt first, RanIt last, Compare comp = Compare())
	{
		_auxSort(first, last, last - first, comp);
	}

	template<typename RanIt, typename Compare = std::less<>>
	void nthElement(RanIt first, RanIt nth, RanIt last, Compare comp = Compare())
	{
		//while (last - first > MAX_INSERT_ELE) {
		while (true) {
			RanIt pivot = _auxPartition(first, last, comp);
			if (pivot < nth) {
				first = pivot + 1;
			}
			else if (pivot > nth) {
				last = pivot;
			}
			else {
				return;
			}
		}

		//_auxInsertionSort(first, last, comp);
	}


}
