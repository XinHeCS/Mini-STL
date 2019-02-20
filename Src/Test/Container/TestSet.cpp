#include <Container/Set.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <Alloc/Allocator.h>
#include <iostream>
#include <functional>

using MSTD::Set;
using MSTD::MultiSet;
using std::cout;
using std::endl;

template<typename Key>
void printSet(const Set<Key> &r)
{
	cout << "Set Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << val << ' ';
	}
	cout << endl;
}

template<typename Key>
void printMultiSet(const MultiSet<Key> &r)
{
	cout << "Set Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << val << ' ';
	}
	cout << endl;
}

void testSet()
{
	Set<int> s{ 4, 5, 6, 7, 5, 3, 1, 8, 9, 6, 2 };
	printSet(s);
	MultiSet<int> s1{ 4, 5, 6, 7, 5, 3, 1, 8, 9, 6, 2 };
	printMultiSet(s1);
	s1.erase(5);
	printMultiSet(s1);

	Set<int> s2{ 7, 5, 3, 1, 8, 9, 6, 2, 4, 5, 6 };
	cout << std::boolalpha;
	cout << (s == s2) << endl;
}