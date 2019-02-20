#include <Container/Internal/_RBTree.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <Alloc/Allocator.h>
#include <iostream>
#include <functional>

using MSTD::_RBTree;
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
		MULTI = true
		//MULTI = false
	};

	static const KeyType& getKeyFromVal(const ValueType &val)
	{
		return val;
	}
};

template<typename T>
void printTree(const _RBTree<T> &r)
{
	cout << "Tree Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << val << ' ';
	}
	cout << endl;
}

void testTree()
{
	_RBTree<Config> t{ 5, 4, 8, 1, 2, 9, 4, 10, 15, 7 };
	printTree(t);
	cout << t.count(4) << endl;

	_RBTree<Config> t1(t);
	printTree(t1);

	//t1.clear();
	t1.erase(t1.begin(), t1.end());
	printTree(t1);
	t1 = t;
	printTree(t1);

	_RBTree<Config> t2;
	t2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	printTree(t2);

	auto it = t2.find(17);
	t2.erase(it);
	t2.erase(16);
	printTree(t2);

	t1.swap(t2);
	printTree(t1);
	printTree(t2);
}