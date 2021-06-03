//============================================================================
// Name        : DesignByContract.h
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : Declarations for design by contract in C++
//============================================================================

#include <assert.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define REQUIRE(assertion, what) \
        if (!(assertion)) _assert (what, __FILE__, __LINE__)

#define ENSURE(assertion, what) \
        if (!(assertion)) _assert (what, __FILE__, __LINE__)

#else
#define REQUIRE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__)

#define ENSURE(assertion, what) \
        if (!(assertion)) __assert (what, __FILE__, __LINE__)
#endif