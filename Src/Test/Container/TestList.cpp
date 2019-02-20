#include <Container/List.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <iostream>
#include <Container/Deque.h>

class TestClass
{
public:
	using valueType = int;
	int a = 23;

	int func() const
	{
		return 1;
	}
};

using MSTD::List;
using std::cout;
using std::endl;

template<typename T>
void printList(const List<T> &li)
{
	cout << "List Size: " << li.size() << endl;
	for (auto &&val : li) {
		cout << val << ' ';
	}
	cout << endl;
}

void testList()
{
	List<int> li{ 1, 2, 3, 4, 5 };
	printList(li);

	cout << "Front: " << li.front() << endl;
	cout << "Back: " << li.back() << endl;

	List<int> li2(li.begin(), li.end());
	for (auto it = li2.rbegin(); it != li2.rend(); ++it) {
		cout << *it << ' ';
	}
	cout << endl;

	List<int> li3(9, 12);
	printList(li3);

	List<int> li4(li3);
	printList(li4);

	List<int> li5(li3.begin(), li3.end());
	printList(li5);

	List<int> li6;
	li6 = li;
	printList(li6);
	li6 = li5;
	printList(li6);

	List<int> li7;
	li7 = li5;
	printList(li7);

	li7.clear();
	printList(li7);

	auto li7It = li7.insert(li7.begin(), 3, 19);
	printList(li7);
	li7.insert(li7.begin(), li.begin(), li.end());
	printList(li7);
	li7.insert(li7It, {11, 22, 33});
	printList(li7);

	while (li7It != li7.end()) {
		li7It = li7.erase(li7It);
	}
	printList(li7);

	li7.pushBack(123);
	li7.pushBack(123);
	printList(li7);
	cout << li7.emplaceBack(1234) << endl;
	printList(li7);
	li7.popBack();
	printList(li7);

	li7.pushFront(123);
	li7.pushFront(123);
	printList(li7);
	cout << li7.emplaceFront(1234) << endl;
	printList(li7);
	li7.popFront();
	printList(li7);

	li7.resize(15, 5);
	printList(li7);
	li7.resize(5);
	printList(li7);

	li7.swap(li6);
	printList(li6);
	printList(li7);

	List<int> li8{ 1, 3, 5, 7, 9 };
	List<int> li9{ 2, 4, 6, 8, 10, 12 ,14 };
	cout << std::boolalpha;
	cout << (li8 == li8) << ' ' << (li8 < li9) << ' ' 
		<< (li9 <= li8) << ' ' << (li8 <= li9) << endl;
	li8.merge(li9);
	printList(li8);
	printList(li9);

	printList(li5);
	printList(li6);
	li5.splice(li5.begin(), li6, li6.begin(), li6.end());
	printList(li5);
	printList(li6);
	li5.remove(12);
	printList(li5);

	li5.removeIf([](int val) { return val % 2; });
	printList(li5);

	li.reverse();
	printList(li);

	List<int> li1{ 1, 2, 3, 3, 4, 5, 5, 5, 7 };
	li1.unique();
	printList(li1);

	List<int> li10{ 4, 1, 2, 5, 3, 7, 9, 6, 4, 1, 10 };
	li10.sort();
	printList(li10);

	cout << (li1 < li10) << endl;
}