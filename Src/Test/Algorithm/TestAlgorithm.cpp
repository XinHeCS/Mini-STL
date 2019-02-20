#include <Algorithm/Algorithm.h>
#include <Algorithm/Numeric.h>
#include <Container/Vector.h>
#include <Container/List.h>
#include <iostream>
#include <random>

using MSTD::Vector;
using MSTD::List;
using std::cout;
using std::endl;
using std::default_random_engine;
using std::uniform_int_distribution;

template<typename Container>
void printAlgorithmRet(Container &vec)
{
	cout << "Container Size: " << vec.size() << endl;
	for (auto val : vec) {
		cout << val << ' ';
	}
	cout << endl;
}

void testNumeric()
{
	Vector<int> vec{ 4, 4, 5, 8, 9, 6, 7, 2, 1, 3 };
	auto ret = MSTD::accumulate(vec.begin(), vec.end(), 0);
	cout << "Sum of vec: " << ret << endl;

	Vector<int> vec3{ 1, 2, 3, 4, 5 };
	cout << "vec3:  " << endl;
	printAlgorithmRet(vec3);
	cout << "Inner product of vec3 and vec3" << endl;
	cout << MSTD::innerProduct(vec3.begin(), vec3.end(), vec3.begin(), vec3.end(), 0) << endl;

	Vector<int> vec4;
	Vector<int> vec2;
	cout << "Partial sum of vec3 :" << endl;
	MSTD::partialSum(vec3.begin(), vec3.end(), MSTD::backInserter(vec4));
	printAlgorithmRet(vec4);
	cout << "Adjacent difference of partial sum of vec3 :" << endl;
	MSTD::adjacentDifference(vec4.begin(), vec4.end(), MSTD::backInserter(vec2));
	printAlgorithmRet(vec2);

	MSTD::iota(vec.begin(), vec.end(), 0);
	cout << "New vec: " << endl;
	printAlgorithmRet(vec);
}

void testSetOperation()
{
	Vector<int> vec1{ 1, 2, 3, 5, 6, 7, 7, 9 };
	Vector<int> vec2{ 1, 2, 4, 6, 8, 10, 10 };
	Vector<int> vec3;
	MSTD::setUnion(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), MSTD::backInserter(vec3));
	cout << "Result of vec1 \\cup vec2: " << endl;
	printAlgorithmRet(vec3);
	Vector<int> vec4;
	MSTD::setIntersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), MSTD::backInserter(vec4));
	cout << "Result of vec1 \\cap vec2: " << endl;
	printAlgorithmRet(vec4);
	Vector<int> vec5;
	cout << "Result of vec1 - vec2: " << endl;
	MSTD::setDifference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), MSTD::backInserter(vec5));
	printAlgorithmRet(vec5);
	Vector<int> vec6;
	cout << "Result of symmetric difference of vec1 and vec2:" << endl;
	MSTD::setSymmetricDifference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), MSTD::backInserter(vec6));
	printAlgorithmRet(vec6);
}

void testHeap()
{
	cout << std::boolalpha;
	Vector<int> vec{ 3, 1, 4, 1, 5, 9 };
	cout << "Heap property ends at: " << endl;
	cout << *MSTD::isHeapUtil(vec.begin(), vec.end()) << endl;
	cout << "Vec is heap: " << MSTD::isHeap(vec.begin(), vec.end()) << endl;

	MSTD::makeHeap(vec.begin(), vec.end());	
	cout << "Heap: " << endl;
	printAlgorithmRet(vec);
	cout << "Vec is heap: " << MSTD::isHeap(vec.begin(), vec.end()) << endl;

	vec.pushBack(10);
	MSTD::pushHeap(vec.begin(), vec.end());
	cout << "New heap:" << endl;
	printAlgorithmRet(vec);

	MSTD::sortHeap(vec.begin(), vec.end());
	cout << "Sorted vector:" << endl;
	printAlgorithmRet(vec);
}

void testBasic()
{
	Vector<int> vec{ 1, 3, 5, 7, 9 };
	Vector<int> vec1{ 2, 4, 6, 8, 10 };
	cout << "Maximum of vec: " << *MSTD::maxElement(vec.begin(), vec.end()) << endl;
	cout << "Minimum of vec: " << *MSTD::minElement(vec.begin(), vec.end()) << endl;

	Vector<int> mergeRet;
	MSTD::merge(vec.begin(), vec.end(), vec1.begin(), vec1.end(), MSTD::backInserter(mergeRet));
	cout << "Merge vec and vec1: " << endl;
	printAlgorithmRet(mergeRet);

	MSTD::reverse(mergeRet.begin(), mergeRet.end());
	cout << "Reverse ret: " << endl;
	printAlgorithmRet(mergeRet);
	MSTD::reverse(vec.begin(), vec.end());
	cout << "Reverse vec: " << endl;
	printAlgorithmRet(vec);

	List<int> li{ 1, 2, 3, 4, 5, 6, 7 };
	auto first = li.begin();
	auto last = li.end();
	auto mid = first;
	MSTD::advance(mid, li.size() / 2);
	printAlgorithmRet(li);
	MSTD::rotate(first, mid, last);
	printAlgorithmRet(li);

	Vector<int> vec2{ 1, 1, 2, 3, 4, 4, 4, 5, 6 };
	printAlgorithmRet(vec2);
	const auto erasePos = MSTD::unique(vec2.begin(), vec2.end());
	vec2.erase(erasePos, vec2.end());
	cout << "Unique vec2: " << endl;
	printAlgorithmRet(vec2);

	Vector<int> vec3{ 1, 3, 5, 5, 6, 7, 9 };
	auto lb = MSTD::lowerBound(vec3.begin(), vec3.end(), 5);
	cout << "Position: " << lb - vec3.begin() << endl;
	auto ub = MSTD::upperBound(vec3.begin(), vec3.end(), 5);
	cout << "Position: " << ub - vec3.begin() << endl;

	Vector<int> permu{ 1, 3, 3 };	
	int i = 0;
	do {
		cout << "Permutation " << ++i << ":\t";
		for (auto &&val : permu) {
			cout << val << ' ';
		}
		cout << endl;
	}
	while (MSTD::nextPermutation(permu.begin(), permu.end()));

	Vector<int> permu2{ 3, 3, 1 };
	i = 0;
	do {
		cout << "Permutation " << ++i << ":\t";
		for (auto &&val : permu2) {
			cout << val << ' ';
		}
		cout << endl;
	} while (MSTD::prevPermutation(permu2.begin(), permu2.end()));

}

void testSort()
{
	Vector<int> vec;
	default_random_engine e;
	uniform_int_distribution<int> u(0, 100);
	for (int i = 0; i < 12; ++i) {
		vec.pushBack(u(e));
	}

	printAlgorithmRet(vec);
	MSTD::nthElement(vec.begin(), vec.begin() + 5, vec.end());

	printAlgorithmRet(vec);
	MSTD::sort(vec.begin(), vec.end());
	//auto it = MSTD::_auxPartition(vec.begin(), vec.end());
	//cout << "Pivot: " << *it << endl;
	printAlgorithmRet(vec);


}

void testAlgorithm()
{
	//testNumeric();
	//testSetOperation();
	//testHeap();
	//testBasic();
	testSort();
}