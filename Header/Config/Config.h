#pragma once

// Memory leak debug
#ifdef _WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <cstdlib>
	#include <crtdbg.h>
	#ifdef _DEBUG
		#define DEBUG_NEW new(_NORMAL_BLOCK , __FILE__ , __LINE__)
		#define _REPORT_MEM() _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#else
		#define DEBUG_NEW new 
		#define _REPORT_MEM()
	#endif
#else
	#define DEBUG_NEW new 
	#define _REPORT_MEM()
#endif

// Basic functional settings
/* #undef USE_DIRECT_MALLOC */

#define USE_EXCEPTION
#ifdef USE_EXCEPTION
	#define _MSTD_TRY			try {
	#define _MSTD_CATCH(x)		} catch(x) {
	#define _MSTD_CATCH_ALL		} catch(...) {
	#define _MSTD_END_CATCH		}
#else
	#define _MSTD_TRY			if (1) {
	#define _MSTD_CATCH(x)		} else if (0) {
	#define _MSTD_CATCH_ALL		} else if (0) {
	#define _MSTD_END_CATCH		}
#endif

// STL version setting
#define CPP_STD 11

