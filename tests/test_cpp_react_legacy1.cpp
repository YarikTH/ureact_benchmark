#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <react/Domain.h>
#include <react/Signal.h>

TEST_SUITE( "cpp.react_legacy1" )
{

// Macro strictly require react namespace to be exposed they don't work otherwise.
// What the point to have a namespace then?!
using namespace react;

// Macro is forced to be outside of the fuction
REACTIVE_DOMAIN(D, sequential)
USING_REACTIVE_DOMAIN(D)

TEST_CASE( "Trivial" )
{
    VarSignalT<int> b = MakeVar<D>(1);
    VarSignalT<int> c = MakeVar<D>(2);
    SignalT<int> a = b + c;

    CHECK(a.Value() == 3);
    b <<= 10;
    CHECK(a.Value() == 12);
}

}
