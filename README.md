# Mini-STL
A toy C++11 Standard Template Library

## Introduction
This is a toy C++11 standard template library. We implement the most wildly used libraries in C++11, including:

* Fundamental libraries (TypeTraits, Memory Manager, Iterator and some other utilities);
* Sequence Container (Vector, List, Dequeue, Queue, Stack);
* Associative Container (Map, Set, MultiMap, MultiSet, Red-Black Tree, Skip List);
* Algorithms (Commonly used numeric algorithms and some other useful algorithms such as sorting, copying, etc.);
* Unit Test (A simply unit test module to test the functionality and report the statistics information)

We also referred to both [libC++ 9.0.0](https://libcxx.llvm.org/docs/) and [MSVC STL](https://docs.microsoft.com/en-us/previous-versions/cscc687y%28v%3dvs.140%29) during developing Mini-STL and you access to their official websites through above links to get more infomation.

## Usage
* To try Mini-STL in any other projects, you need download the source code and include the proper header file in your source file (<b>Note:</b> Mini-STL is just for fun and please don't put it in any code for industry or commercial use).
* To run the unit test code: 1. you need install [CMake(>= 3.0.0)](https://cmake.org/download/) and any C++ compile in [this list](https://en.wikipedia.org/wiki/Pragma_once#Portability) according to your operating system. 2. Build and compile this project in root directory. 3. Run the binary file named Demo to start unit test.

## Configuration
You can enable or disable some functionalities when playing with Mini-STL. All the configuration options can be modified in CMakeLists.txt of rooted directory.

<!-- In CMakeLists.txt, set: -->
~~~
# Cpp standard setting
SET(CPP_STD 11)
~~~
* When CPP_STD == 11, Mini-STL will run in C++11 standard;
* When CPP_STD == 14, Mini-STL will run in C++14 standard (Only partly supported);
* When CPP_STD == 17, Mini-STL will run in C++17 standard (Only partly supported);

~~~
# Memory manage strategy options
option(USE_DIRECT_MALLOC
		"Whether to use simple malloc strategy" OFF)
~~~
* When switch it to 'on', Mini-STL will use build-in memory allocation rule on your system;
* When switch it to 'off', Mini-STL will maintain a internal memory pool to support memory allocation;

~~~
option(USE_EXCEPTION
		"Whether to use exception handling" ON)
~~~
* Set to 'on' to enable Exception Handling in Mini-STL;
* Set to 'off' to disable Exception Handling in Mini-STL;

## Licience
Mini-STL is under [MIT](https://opensource.org/licenses/MIT) licience.