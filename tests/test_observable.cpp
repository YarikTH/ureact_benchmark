#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <observable/observable.hpp>

TEST_SUITE( "observable" )
{

TEST_CASE( "Trivial" )
{
    observable::value<int> b{ 1 };
    observable::value<int> c{ 2 };

    SUBCASE("Not reactive by default")
    {
        auto a = b + c;
        
        CHECK(a.get() == 3);
        b = 10;
        
        CHECK(a.get() == 3); // contains old value
        a.eval(); // need to call recalculation manually. Not quite reactive
    
        CHECK(a.get() == 12);
    }
    
    SUBCASE("Reactive")
    {
        observable::expression<int, observable::immediate_evaluator> a( b + c );
        
        CHECK(a.get() == 3);
        b = 10;
        
        CHECK(a.get() == 12);
    }
    
    //auto a = b + c;
    
#if 0
    // Expressions are quite ugly
    observable::expression<int, observable::immediate_evaluator> a( observable::expression_node<int> {
        [](auto a, auto b) { return a + b; },
        observable::expression_node<int> { b },
        observable::expression_node<int> { c }
    } );
#endif
}

}
