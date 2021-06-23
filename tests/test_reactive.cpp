#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <reactive/non_thread_safe/ObservableProperty.h>
#include <reactive/non_thread_safe/ReactiveProperty.h>

TEST_SUITE( "reactive" )
{

    TEST_CASE( "Trivial" )
    {
        using namespace reactive::non_thread_safe;

        ObservableProperty<int> b{ 1 };
        ObservableProperty<int> c{ 2 };
        // b + c doesn't work. Result is a constant 2. Becase it is bool(b) + bool(c)
        // ReactiveProperty<int> a = b + c;
        ReactiveProperty<int> a{ -1 };
        a.set( []( int b, int c ) { return b + c; }, b, c );

        CHECK( a.getCopy() == 3 );
        b = 10;
        CHECK( a.getCopy() == 12 );
    }
}
