#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <urp.hpp>

TEST_SUITE( "urp" )
{

    TEST_CASE( "Trivial" )
    {
        using namespace usingstdcpp2019;

        // A way more cleaner syntax. Short namespace, omited types, clear names
        urp::value b = 1;
        urp::value c = 2;
        urp::function a = b + c;

        CHECK( a.get() == 3 );
        b = 10;
        CHECK( a.get() == 12 );
    }
}
