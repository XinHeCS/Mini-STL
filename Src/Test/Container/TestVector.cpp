#include <Container/Vector.h>
#include <iostream>
#include "../TestUtility.h"

using MSTD::Vector;
using std::cout;
using std::endl;

void testVector()
{
	Vector<int> vec1(Vector<int>{1, 2, 3, 4, 5});
	Vector<int> vec(vec1);

	EXPECT_CONTAINER_EQ(vec1, vec, "Copy construct function test failed");

	auto it = vec.begin();
	auto cit = vec.cbegin();

	EXPECT_BASE_EQ(*(it), *(cit), "const iterator test failed");
	EXPECT_BASE_EQ(*(it + 3), *(cit + 3), "Iterator random access test failed");

	it = vec.end();
	EXPECT_BASE_EQ(it, vec.end(), "Fetch end position failed");
	it = vec.insert(it, 6);
	EXPECT_BASE_EQ(vec.back(), *it, "Insert at back failed");

	Vector<int> arrV;
	int arr[3] = { 7, 8, 9 };

	arrV.insert(arrV.begin(), arr, arr + 3);
	EXPECT_RANGE_EQ(arrV.begin(), arrV.end(), arr, arr + 3, "Insertion range failed");

	//Vector<int> vec2{ 1, 2, 3, 4, 5 };
	//Vector<int> vec2Insert{ 1, 2, 3, 1, 2, 3, 4, 5 };
	//vec2.insert(vec2.begin(), vec2.begin(), vec2.begin() + 3);

	vec.reserve(20);
	EXPECT_BASE_EQ(vec.size(), 6, "Size test after reserve failed");
	EXPECT_BASE_EQ(vec.capacity(), 20, "Capacity test after reserve failed");

	vec.shrinkToFit();
	EXPECT_BASE_EQ(vec.size(), 6, "Size test after reserve failed");
	EXPECT_BASE_EQ(vec.capacity(), 6, "Capacity test after reserve failed");

	vec.erase(vec.begin());
	Vector<int> vecErased{ 2, 3, 4, 5, 6 };
	EXPECT_CONTAINER_EQ(vec, vecErased, "Erase test failed");
	
	vec.erase(vec.begin(), vec.begin() + 3);
	Vector<int> vecEraseRange{ 5, 6 };
	EXPECT_CONTAINER_EQ(vec, vecEraseRange, "Erase range failed");

	Vector<int> vec2;
	Vector<int> vec2Push{ 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 };
	for (auto i = 0; i < 5; ++i) {
		vec2.pushBack(i);
		vec2.emplaceBack(i);
	}
	EXPECT_CONTAINER_EQ(vec2, vec2Push, "Push back failed");

	vec2.swap(vec);
	Vector<int> vec2AfterSwap{ 5, 6 };
	EXPECT_BASE_EQ(vec2, vec2AfterSwap, "Swap test failed");

	EXPECT_BASE_EQ(vec, vec, "operator== test failed");
	EXPECT_BASE_NEQ(vec, vec2, "operator== test failed");
}