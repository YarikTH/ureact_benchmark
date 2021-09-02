#include <benchmark/benchmark.h>
#include <react/Domain.h>
#include <react/Signal.h>

#include "macros.hpp"

// Macro strictly require react namespace to be exposed they don't work otherwise.
// What the point to have a namespace then?!
using namespace react;

// Macro is forced to be outside of the fuction
REACTIVE_DOMAIN( D, sequential )
USING_REACTIVE_DOMAIN( D )

static void cpp_react_legacy1_chained_functions_baseline( benchmark::State& state )
{
    auto a = MakeVar<D>( 1 );
    auto d = MakeSignal<D>( a,
        []( int a ) { //
            return ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );
        } );

    assert( d.Value() == 8 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( d.Value() == 8 * i * i );
        benchmark::DoNotOptimize( d.Value() );
        ++i;
    }
}
BENCHMARK( cpp_react_legacy1_chained_functions_baseline )
    ->Name( FULL_BENCHMARK_NAME( cpp_react_legacy1_chained_functions_baseline ) );

static void cpp_react_legacy1_chained_functions_separate( benchmark::State& state )
{
    auto a = MakeVar<D>( 1 );

    auto b1 = MakeSignal<D>( With( a, a ), std::plus<>() );
    auto b2 = MakeSignal<D>( With( a, a ), std::plus<>() );
    auto b3 = MakeSignal<D>( With( a, a ), std::plus<>() );
    auto b4 = MakeSignal<D>( With( a, a ), std::plus<>() );

    auto c1 = MakeSignal<D>( With( b1, b2 ), std::multiplies<>() );
    auto c2 = MakeSignal<D>( With( b3, b4 ), std::multiplies<>() );

    auto d = MakeSignal<D>( With( c1, c2 ), std::plus<>() );

    assert( d.Value() == 8 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( d.Value() == 8 * i * i );
        benchmark::DoNotOptimize( d.Value() );
        ++i;
    }
}
BENCHMARK( cpp_react_legacy1_chained_functions_separate )
    ->Name( FULL_BENCHMARK_NAME( cpp_react_legacy1_chained_functions_separate ) );

static void cpp_react_legacy1_chained_functions_optimized( benchmark::State& state )
{
    auto a = MakeVar<D>( 1 );
    auto d = ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );

    assert( d.Value() == 8 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( d.Value() == 8 * i * i );
        benchmark::DoNotOptimize( d.Value() );
        ++i;
    }
}
BENCHMARK( cpp_react_legacy1_chained_functions_optimized )
    ->Name( FULL_BENCHMARK_NAME( cpp_react_legacy1_chained_functions_optimized ) );
