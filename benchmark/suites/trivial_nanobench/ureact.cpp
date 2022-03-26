#include <nanobench.h>
#include <ureact/ureact.hpp>

#include "macros.hpp"

int main()
{
    {
        ankerl::nanobench::Bench().run("ureact context construction", [&] {
            ureact::context ctx;
            ankerl::nanobench::doNotOptimizeAway(ctx);
        });
    }

    {
        ureact::context ctx;
        
        ankerl::nanobench::Bench().run("ureact var_signal construction", [&] {
            auto a = ctx.make_value( 1 );
            ankerl::nanobench::doNotOptimizeAway(a);
        });
    }
    
    {
        ureact::context ctx;
        auto a = ctx.make_value( 1 );
        
        ankerl::nanobench::Bench().run("ureact signal construction", [&] {
            auto b = ureact::make_function( a, std::negate<>() );
            assert( b.get() == -1 );
            ankerl::nanobench::doNotOptimizeAway(b);
        });
    }

    {
        ureact::context ctx;
        auto a = ctx.make_value( 1 );
        auto b = ureact::make_function( a, std::negate<>() );
        assert( b.get() == -1 );
        int i = 0;

        ankerl::nanobench::Bench().run("ureact signal reaction", [&] {
            a <<= i;
            assert( b.get() == -i );
            ++i;
        });
    }

    {
        ureact::context ctx;
        auto a = ctx.make_value( 1 );
        auto d = ureact::make_function( a,
            []( int a ) { //
                return ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );
            } );
        assert( d.get() == 8 );

        int i = 0;

        ankerl::nanobench::Bench().run("ureact signal functions baseline", [&] {
            a <<= i;
            assert( d.get() == 8 * i * i );
            ankerl::nanobench::doNotOptimizeAway( d.get() );
            ++i;
        });
    }
    
    {
        ureact::context ctx;
        auto a = ctx.make_value( 1 );
    
        auto b1 = ureact::make_function( with( a, a ), std::plus<>() );
        auto b2 = ureact::make_function( with( a, a ), std::plus<>() );
        auto b3 = ureact::make_function( with( a, a ), std::plus<>() );
        auto b4 = ureact::make_function( with( a, a ), std::plus<>() );
    
        auto c1 = ureact::make_function( with( b1, b2 ), std::multiplies<>() );
        auto c2 = ureact::make_function( with( b3, b4 ), std::multiplies<>() );
    
        auto d = ureact::make_function( with( c1, c2 ), std::plus<>() );
    
        assert( d.get() == 8 );
    
        int i = 0;

        ankerl::nanobench::Bench().run("ureact context separate", [&] {
            a <<= i;
            assert( d.get() == 8 * i * i );
            ankerl::nanobench::doNotOptimizeAway( d.get() );
            ++i;
        });
    }
    
    {
        ureact::context ctx;
        auto a = ctx.make_value( 1 );
        auto d = ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );

        assert( d.get() == 8 );

        int i = 0;

        ankerl::nanobench::Bench().run("ureact context optimized", [&] {
            a <<= i;
            assert( d.get() == 8 * i * i );
            ankerl::nanobench::doNotOptimizeAway( d.get() );
            ++i;
        });
    }
}
