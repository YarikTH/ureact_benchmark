#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// For some unknown reason method State::Value() is protected. So we need a
// "public morozov" hack to operate it
#define protected public
#include <react/group.h>
#include <react/state.h>

TEST_SUITE( "cpp.react_master" )
{

    TEST_CASE( "Trivial" )
    {
        react::Group group;

        // Creation of vars and states become much move verbose and uglier than in legacy1.
        // Also, it seems that operator overloads were removed but not restored later.
        auto b = react::StateVar<int>::Create( group, 1 );
        auto c = react::StateVar<int>::Create( group, 2 );
        auto a = react::State<int>::Create( []( int b, int c ) { return b + c; }, b, c );

        CHECK( a.Value() == 3 );
        b.Set( 10 ); // I'm not sure if there is overload operator for assigment
        CHECK( a.Value() == 12 );
    }
}
