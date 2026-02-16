// -------------------------------------------------------------
//
//!\file test_example_functions.cpp
//!\brief Unit tests for EXAMPLE output behavior.
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------
//
//            Copyright (c) 2026. Colin J.D. Stewart
//                   All rights reserved
//
// -------------------------------------------------------------

// SUT header
#include "example.hpp"

// shocktest
#include "shocktest.hpp"

//
//!\brief Verifies exampleFunction1 writes the expected stdout message.
//
SHOCKTEST_CASE(ExampleFunction1WritesExpectedMessage)
{
    EXPECT_STDCOUT(EXAMPLE::exampleFunction1(), "This is an example function in the library.\n");
}

//
//!\brief Verifies exampleFunction2 writes the expected stdout message.
//
SHOCKTEST_CASE(ExampleFunction2WritesExpectedMessage)
{
    EXPECT_STDCOUT(EXAMPLE::exampleFunction2(), "This is another example function in the library.\n");
}
