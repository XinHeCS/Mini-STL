#include <Config/Config.h>

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <tuple>
#include <string>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <type_traits>
#include <functional>
#include "Alloc/MiniAlloc.h"
//
//using namespace std;
//
//void testAlloc(size_t n)
//{
//	MSTD::Allocator<int> alloc;
//
//	auto p = alloc.allocate(n);
//
//	auto start = p;
//	for (auto i = 0; i < n; ++i) {
//		alloc.construct(start + i, i);
//	}
//
//	for (auto i = 0; i < n; ++i) {
//		std::cout << *(start + i) << std::endl;
//	}
//
//	for (auto i = 0; i < n; ++i) {
//		alloc.destroy(start + i);
//	}
//
//	alloc.deallocate(start, 8);
//}
//
//class Pointer
//{
//public:
//	using valueType = int;
//	static constexpr int x = 1312;
//	int a = 23;
//
//	int func() const
//	{
//		return 1;
//	}
//};
//
//class Pointer2 : private Pointer
//{
//public:
//	enum 
//	{
//		value = 12
//	};
//};
//
//void funcOut()
//{
//	//std::cout << a << std::endl;
//}

//int main()
//{
//	std::cout << "Version: " 
//		<< STL_VERESION_MAJOR << "."
//		<< STL_VERESION_MID << '.'
//		<< STL_VERESION_MINOR << std::endl;	
//
//#ifdef USE_DIRECT_MALLOC
//	std::cout << "Very Good!" << std::endl;
//#else
//	std::cout << "Oops!" << std::endl;
//#endif // USE_DIRECT_MALLOC
//
//	MSTD::isSame<int&, int>::value;
//	MSTD::removeCV<const int>::type i;
//	MSTD::isVoid<void*>::value;
//	MSTD::addConst<int*>::type x = &i;
//	MSTD::addCV<int>::type c = i;
//	MSTD::isIntegral<float>::value;
//	MSTD::isFloating<float>::value;
//	MSTD::isArray<int[12]>::value;
//
//	enum Test
//	{
//		aa, bb, cc
//	};
//
//	std::cout << MSTD::isClass<int>::value;
//
//	MSTD::isMemberPointer<Test Pointer::*>::value;
//
//	//MSTD::isFunction<int ()>::value;
//
//	//MSTD::isMemberDataPointer<int (Pointer::*)()>::value;
//	//MSTD::isMemberDataPointer<int Pointer::*>::value;
//	//MSTD::isMemberDataPointer<int>::value;	
//	//
//	//std::cout << typeid(
//	//	MSTD::isMemberFunctionPointer<int (Pointer::*)()>)
//	//	.name() << std::endl;
//	//MSTD::isMemberFunctionPointer<int (Pointer::*)()>::value;
//	//MSTD::isMemberFunctionPointer<int Pointer::*>::value;
//	//MSTD::isMemberFunctionPointer<int>::value;
//	//MSTD::isMemberFunctionPointer<int ()>::value;
//
//	//MSTD::isArithmetic<const int>::value;
//	//MSTD::isArithmetic<Pointer>::value;
//	//MSTD::isArithmetic<float>::value;
//	//MSTD::isArithmetic<float (int)>::value;
//
//	//MSTD::isScalar<const int>::value;
//	//MSTD::isScalar<int Pointer::*>::value;
//	//MSTD::isScalar<float>::value;
//	//MSTD::isScalar<float(int)>::value;
//
//	//MSTD::isObject<const int>::value;
//	//MSTD::isObject<Pointer>::value;
//	//MSTD::isObject<float>::value;
//	//MSTD::isObject<float(int)>::value;
//
//	//MSTD::isReference<int>::value;
//	//MSTD::isReference<int&>::value;
//	//MSTD::isReference<const int&&>::value;
//
//	////MSTD::isSigned<Pointer>::value;
//	//MSTD::isSigned<int>::value;
//	//MSTD::isSigned<unsigned int>::value;
//
//	//decltype(std::declval<>()) what;
//	//std::vector<int> vec(std::string());
//	//std::cout << std::boolalpha;
//	//std::cout << MSTD::isConstructible<std::vector<int>, int>::value << std::endl;
//	//std::cout << MSTD::isConstructible<Pointer, int>::value << std::endl;
//	//std::cout << MSTD::isConstructible<Pointer>::value << std::endl;
//	//std::cout << MSTD::isConstructible<void, int*>::value << std::endl;
//	//std::cout << std::is_constructible<void, int*>::value << std::endl;
//	//std::cout << MSTD::isConstructible<int*, int>::value << std::endl;
//	//std::cout << std::is_constructible<int*, int>::value << std::endl;
//	//std::cout << MSTD::isConstructible<void*, int*>::value << std::endl;
//	//std::cout << std::is_constructible<void*, int*>::value << std::endl;
//	//std::cout << MSTD::isConstructible<void, void>::value << std::endl;
//	//std::cout << std::is_constructible<void, void>::value << std::endl;
//
//	//MSTD::alignmentOf<Pointer>::value;
//
//	//MSTD::rank<int*>::value;
//	//MSTD::rank<int[][12][12]>::value;
//	//MSTD::rank<int[]>::value;
//	//std::rank<int[]>::value;
//	//std::rank<int[][12][12]>::value;
//	//std::rank<int*>::value;
//
//	//std::extent<int[][13][14], 1>::value;
//	//MSTD::extent<int[][13][14], 1>::value;	
//	//std::extent<int[][13][14], 2>::value;
//	//MSTD::extent<int[][13][14], 2>::value;	
//	//std::extent<int[][13][14], 0>::value;
//	//MSTD::extent<int[][13][14], 0>::value;	
//	//std::extent<int[][13][14], 10>::value;
//	//MSTD::extent<int[][13][14], 10>::value;	
//	//std::extent<int, 0>::value;
//	//MSTD::extent<int, 0>::value;
//
//	//std::cout << MSTD::isConvertible<int, double>::value << std::endl;
//	//std::cout << std::is_convertible<int, double>::value << std::endl;
//	//std::cout << MSTD::isConvertible<int, int&>::value << std::endl;
//	//std::cout << std::is_convertible<int, int&>::value << std::endl;
//	//std::cout << MSTD::isConvertible<int&&, int>::value << std::endl;
//	//std::cout << std::is_convertible<int&&, int>::value << std::endl;
//	std::cout << MSTD::isConvertible<Pointer2*, Pointer*>::value << std::endl;
//	std::cout << std::is_convertible<Pointer2*, Pointer*>::value << std::endl;
//	std::cout << MSTD::isConvertible<Pointer2*, Pointer>::value << std::endl;
//	std::cout << std::is_convertible<Pointer2*, Pointer>::value << std::endl;
//	std::cout << MSTD::isConvertible<void, const volatile void>::value << std::endl;
//	std::cout << std::is_convertible<void, const volatile void>::value << std::endl;
//
//	//MSTD::removeCV<const int&>::type;
//	//std::remove_cv<const int&>::type;
//
//	cout << MSTD::isBasef<Pointer, Pointer2>::value << endl;
//	cout << std::is_base_of<Pointer, Pointer2>::value << endl;
//	cout << MSTD::isBasef<const Pointer, Pointer2>::value << endl;
//	cout << std::is_base_of<const Pointer, Pointer2>::value << endl;	
//	//cout << MSTD::isBasef<int, long>::value << endl;
//	cout << MSTD::isBasef<int, long>::value << endl;
//	cout << std::is_base_of<int, long>::value << endl;
//
//	//test2<const volatile void, void>();
//
//	//is_base_of<Pointer, Pointer2>::value;
//
//	//MSTD::removeReference<int>::type;
//	//MSTD::removeReference<int&>::type;
//	//MSTD::removeReference<int&&>::type;
//	//MSTD::removeReference<const int&&>::type;
//
//	//std::add_lvalue_reference<const int*>::type;
//	//MSTD::addLvalueReference<const int*>::type;
//
//	//std::add_rvalue_reference<int&&>::type;
//	//MSTD::addRvalueReference<int&&>::type;	
//	//std::cout << typeid(std::add_rvalue_reference<const void>::type).name() << std::endl;
//	//MSTD::addRvalueReference<const void>::type;
//
//	//MSTD::removePointer<const int* const>::type;
//
//	std::add_pointer<int&>::type;
//	MSTD::addPointer<void>::type;
//
//	MSTD::makeSigned<char>::type;
//	MSTD::isUnsigned<char>::value;
//	std::is_unsigned<char>::value;
//
//	MSTD::removeExtent<int[][12]>::type;
//	MSTD::removeAllExtents<int[][12]>::type;
//
//	std::decay<const int*>::type;
//	MSTD::decay<int[][12][12]>::type;
//	cout << typeid(std::decay<int[][12][12]>::type).name() << endl;
//
//	cout << typeid(MSTD::decay<int (Pointer::*)() const>::type).name() << endl;
//	/*cout << typeid(std::remove_const<int(int) const>::type).name() << endl;*/
//	cout << typeid(std::decay<int (Pointer::*)() const>::type).name() << endl;
//
//	cout << typeid(MSTD::decay<int>::type).name() << endl;
//	cout << typeid(std::decay<int>::type).name() << endl;
//
//	//cout << typeid(&Pointer::func).name() << endl;
//
//	std::common_type<int, float, bool>::type;
//
//	MSTD::Allocator<int> a;
//
//	using StaticAlloc = MSTD::AllocatorTraits<MSTD::Allocator<int>>;
//
//	auto palloc = StaticAlloc::allocate(a, 1);
//	StaticAlloc::construct(a, palloc, 12);
//	cout << *palloc << endl;
//	StaticAlloc::destroy(a, palloc);
//	StaticAlloc::deallocate(a, palloc, 1);
//
//	return 0;
//}

extern void testVector();
extern void testList();
//extern void testDeque();
extern void testPQ();
extern void testTree();
extern void testMap();
extern void testSet();
extern void testSkipList();

extern void testAlgorithm();


int main()
{
	_REPORT_MEM();
	testVector();
	//testList();
	//testPQ();
	//testTree();
	//testMap();
	//testSet();
	//testSkipList();

	//testAlgorithm();

	return 0;
}