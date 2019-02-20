#pragma once

// Header for unit test

#include <iostream>

namespace MSTD {

	// Singleton to records the test cases
	class TestCounter
	{
	public:
		TestCounter(const TestCounter &other) = delete;
		TestCounter(TestCounter &&other) = delete;

		TestCounter& operator=(const TestCounter &other) = delete;
		TestCounter& operator=(TestCounter &&other) = delete;

		static TestCounter& getInstance()
		{
			static TestCounter instance;
			return  instance;
		}

		void newTest()
		{
			++_testCount;
		}

		void passTest()
		{
			++_testPass;
		}

		void reportResult() const
		{
			std::cout << "Unit Test finish!" << std::endl;
			std::cout << _testPass << '/' << _testCount
					<< " (" << static_cast<double>(_testPass * 100) / static_cast<double>(_testCount)
					<< '%' << ") passed." << std::endl;
		}

	protected:
		TestCounter() = default;

	private:
		unsigned int _testCount = 0;
		unsigned int _testPass = 0;
	};


#define EXPECT_BASE(condition, err) \
	do \
	{ \
		MSTD::TestCounter::getInstance().newTest();;\
		if (condition) {\
			MSTD::TestCounter::getInstance().passTest(); \
		}\
		else { \
			std::cerr << __FILE__ << ":" << __LINE__ << ' '  << err << std::endl; \
		}\
	} while (0)

#define EXPECT_BASE_EQ(c1, c2, err) EXPECT_BASE((c1) == (c2), err)

#define EXPECT_BASE_NEQ(c1, c2, err) EXPECT_BASE((c1) != (c2), err)

#define EXPECT_CONTAINER_EQ(c1, c2, err) \
	do\
	{	 \
		bool eqFlag = true;\
		auto it1 = c1.begin();\
		auto it2 = c2.begin();\
		while (it1 != c1.end() && it2 != c2.end()) {\
			if (*it1++ != *it2++) {\
				eqFlag = false;\
				break;\
			}\
		}\
		EXPECT_BASE(eqFlag && it1 == c1.end() && it2 == c2.end(), err);\
	} while (0)

#define EXPECT_RANGE_EQ(rs1, re1, rs2, re2, err)\
	do\
	{	 \
		auto s1 = rs1;\
		auto s2 = rs2;\
		auto e1 = re1;\
		auto e2 = re2;\
		bool eqFlag = true; \
		while (s1 != e1&& s2 != e2) { \
			if (*s1++ != *s2++) {\
				eqFlag = false; \
				break; \
			}\
		}\
		EXPECT_BASE(eqFlag && s1 == e1 && s2 == e2, err); \
	} while (0)
}