#pragma once
#include <Iterator/Iterator.h>

// Numeric standard header

namespace MSTD {
	
	/**
	 * Accumulate elements in range [first, last)
	 * 
	 * @param first	- Start position of range	 
	 * @param last	- End position of range
	 * @param init	- Initial value	
	 *	
	 * @return
	 *	The sum of {init} + \sum_{first, last}
	 */
	template<typename InputIt, typename T>
	T accumulate(InputIt first, InputIt last, T init)
	{
		for (; first != last; ++first) {
			init += *first;
		}
		return init;
	}

	// Generic version of accumulate Use user define binary operator 
	// instead of numeric plus.
	template<typename InputIt, typename T, typename BinaryOp>
	T accumulate(InputIt first, InputIt last, T init, BinaryOp op)
	{
		for (; first != last; ++first) {
			init = op(init, *first);
		}
		return init;
	}

	/**
	 * Calculate difference between range [first, last)
	 *
	 * @param first	- Start position of range
	 * @param last	- End position of range
	 * @param dest	- Destination to store the result
	 * @param op	- User defined operator
	 *
	 * @return
	 *	The past to last element of dest
	 */
	template<typename InputIt, typename OutputIt, typename BinaryOp = std::minus<>>
	OutputIt adjacentDifference(InputIt first, InputIt last, OutputIt dest, BinaryOp op = BinaryOp())
	{
		if (first == last) {
			return dest;
		}

		using Pointer = typename IteratorTraits<InputIt>::Pointer;
		*dest = *first;
		Pointer pre = MSTD::addressof(*first);
		while (++first != last) {
			Pointer cur = MSTD::addressof(*first);
			*++dest = op(*cur, *pre);
			pre = cur;
		}
		return dest;
	}

	/**
	 * Calculate inner product of two range.
	 *
	 * @param first	- Start position of range
	 * @param last	- End position of range
	 * @param first2 - Start position of range
	 * @param last2	- End position of range
	 * @param init	- Initial value of ret
	 * @param multiply	- Multiplication op
	 * @param add	- Addition op
	 *
	 * @return
	 *	The inner product of type T
	 */
	template<typename InputIt, 
			 typename T,
			 typename Multiply = std::multiplies<>,
			 typename Add = std::plus<>>
	T innerProduct(InputIt first, InputIt last, InputIt first2, InputIt last2, T init,
					Multiply multiply = Multiply(), Add add = Add())
	{
		for (; first != last; ++first, ++first2) {
			init = add(init, multiply(*first, *first2));
		}
		return init;
	}

	/**
	 * Calculate partial sum of between range [first, last)
	 *
	 * @param first	- Start position of range
	 * @param last	- End position of range
	 * @param dest	- Destination to store the result
	 * @param op	- User defined operator
	 *
	 * @return
	 *	The past to last element of dest
	 */
	template<typename InputIt, 
			 typename OutputIt,
			 typename BinaryOp = std::plus<>>
	OutputIt partialSum(InputIt first, InputIt last, OutputIt dest, BinaryOp op = BinaryOp())
	{
		using Value = typename  IteratorTraits<InputIt>::ValueType;

		if (first == last) {
			return dest;
		}

		*dest = *first;
		Value val = *first;
		while (++first != last) {
			val = op(val, *first);
			*++dest = val;
		}

		return dest;
	}

	/**
	 * Fill the range [first, last) with increasing sequence starting at {val}
	 *
	 * @param first	- Start position of range
	 * @param last	- End position of range
	 * @param val	- Initial value	 
	 *
	 * @return
	 *	The past to last element of dest
	 */
	template<typename ForwardIt, typename T>
	void iota(ForwardIt first, ForwardIt last, T val)
	{
		while (first != last) {
			*first++ = val++;
		}
	}

}
