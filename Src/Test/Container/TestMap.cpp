#include <Container/Map.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <Alloc/Allocator.h>
#include <string>
#include <utility>
#include <iostream>
#include <functional>

using MSTD::Map;
using MSTD::MultiMap;
using std::string;
using std::pair;
using std::cout;
using std::endl;

template<typename Key, typename Val>
void printMap(const Map<Key, Val> &r)
{
	cout << "Map Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << "(" << val.first << ", " << val.second << ")" << "   ";
	}
	cout << endl;
}

template<typename Key, typename Val>
void printMultiMap(const MultiMap<Key, Val> &r)
{
	cout << "Map Size: " << r.size() << endl;
	for (auto &&val : r) {
		cout << "(" << val.first << ", " << val.second << ")" << "   ";
	}
	cout << endl;
}

void testMap()
{
	Map<int, string> m{
		{9, "nine"}, {2, "two"}, {5, "five"},
		{1, "one"}, {3, "three"}, {6, "six"},
		{8, "eight"}, {4, "four"}, {7, "seven"}, {7, "seven"}
	};
	m.insert({ 10, "ten" });
	printMap(m);

	MultiMap<int, string> m2{
		{9, "nine"}, {2, "two"}, {5, "five"},
		{1, "one"}, {3, "three"}, {6, "six"},
		{8, "eight"}, {4, "four"}, {7, "seven"}, {7, "seven"}
	};
	printMultiMap(m2);
	auto pos1 = m2.lowerBound(7);
	auto pos2 = m2.upperBound(7);
	cout << m2.count(7) << endl;
	cout << pos1->second << endl;
	cout << pos2->second << endl;

	Map<string, int> table{
		{"Mike", 3}, {"Frank", 5}, {"Jessy", 3}, {"Mikey", 4}
	};
	printMap(table);
	table["Mike"] = 6;
	++table["Frank"];
	++table["Hue"];
	printMap(table);
}

