#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <ureact/ureact.hpp>

TEST_SUITE( "ureact" )
{

    TEST_CASE( "Trivial" )
    {
        ureact::context ctx;

        ureact::value<int> b = ctx.make_value( 1 );
        ureact::value<int> c = ctx.make_value( 2 );
        ureact::signal<int> a = b + c;

        CHECK( a.get() == 3 );
        b <<= 10;
        CHECK( a.get() == 12 );
    }
}
