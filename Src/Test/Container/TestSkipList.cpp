#include <Container/Internal/_SkipList.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <Alloc/Allocator.h>
#include <iostream>
#include <functional>

using MSTD::_SkipList;
using std::cout;
using std::endl;

struct Config
{
	using DifferenceType = ptrdiff_t;
	using ConstReference = const int&;
	using ConstPointer = const int*;
	using ValueType = int;
	using KeyType = int;
	using Reference = int&;
	using Pointer = int;
	using SizeType = size_t;
	using KeyCompare = std::less<int>;
	using AllocatorType = MSTD::Allocator<int>;

	enum
	{
		//MULTI = true
		MULTI = false
	};

	static const KeyType& getKeyFromVal(const ValueType &val)
	{
		return val;
	}
};

template<typename Config>
void printSkipList(const _SkipList<Config> &r)
{
	cout << "Skip List Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << val << ' ';
	}
	cout << endl;
}

void testSkipList()
{	
	_SkipList<Config> sl{ 5, 9, 4, 7, 1, 3, 2, 6, 8 };
	printSkipList(sl);
	_SkipList<Config> sl2;
	printSkipList(sl2);
	sl2.insert(5);
	sl2.insert(5);
	sl2.insert(9);
	sl2.insert(4);
	sl2.insert(7);
	sl2.insert(1);
	sl2.insert(3);
	sl2.insert(2);
	sl2.insert(6);
	sl2.insert(8);
	printSkipList(sl2);
	sl2.erase(5);
	printSkipList(sl2);
	auto it = sl2.find(6);
	cout << *it << endl;
}