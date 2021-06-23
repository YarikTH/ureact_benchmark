#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <ureact/ureact.hpp>

TEST_SUITE( "ureact" )
{

    TEST_CASE( "Trivial" )
    {
        ureact::context ctx;

        ureact::var_signal<int> b = ctx.make_var( 1 );
        ureact::var_signal<int> c = ctx.make_var( 2 );
        ureact::signal<int> a = b + c;

        CHECK( a.value() == 3 );
        b <<= 10;
        CHECK( a.value() == 12 );
    }
}
