#include <Container/Queue.h>
#include <Container/Vector.h>
#include <TypeInfo/TypeHelper.h>
#include <Iterator/Iterator.h>
#include <iostream>
#include <random>
#include <ctime>

using MSTD::PriorityQueue;
using MSTD::Vector;
using std::cout;
using std::endl;
using std::default_random_engine;
using std::uniform_int_distribution;

template<typename PriQueue>
void printPQ(PriQueue &q)
{
	cout << "PriorityQueue Size: " << q.size() << endl;
	while (!q.empty()) {
		auto val = q.top();
		cout << q.top() << ' ';
		q.pop();
	}
	cout << endl;
}

void testPQ()
{
	PriorityQueue<int, Vector<int>, std::greater<>> q;
	default_random_engine e;
	uniform_int_distribution<int> u(0, 100);
	for (int i = 0; i < 50; ++i) {		
		q.push(u(e));
	}

	printPQ(q);
}