#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <react.hpp>

TEST_SUITE( "react-cpp" )
{

TEST_CASE( "Trivial" )
{
    using namespace react;
    
    // A way more cleaner syntax. Short namespace, omited types, clear names
    auto b = var(1);
    auto c = var(2);
    auto a = b + c; // doesn't work because library is broken

    CHECK(a() == 3);
    b = 10;
    CHECK(a() == 12);
}

}
