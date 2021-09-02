#include <benchmark/benchmark.h>
#include <ureact/ureact.hpp>

#include "macros.hpp"

static void ureact_context_construction( benchmark::State& state )
{
    for( auto _ : state )
    {
        ureact::context ctx;
        benchmark::DoNotOptimize( ctx );
    }
}
BENCHMARK( ureact_context_construction )
    ->Name( FULL_BENCHMARK_NAME( ureact_context_construction ) );


static void ureact_var_signal_construction( benchmark::State& state )
{
    ureact::context ctx;
    for( auto _ : state )
    {
        auto a = ctx.make_value( 1 );
        benchmark::DoNotOptimize( a );
    }
}
BENCHMARK( ureact_var_signal_construction )
    ->Name( FULL_BENCHMARK_NAME( ureact_var_signal_construction ) );


static void ureact_signals_construction( benchmark::State& state )
{
    ureact::context ctx;
    auto a = ctx.make_value( 1 );
    for( auto _ : state )
    {
        auto b = ureact::make_function( a, std::negate<>() );
        assert( b.get() == -1 );
    }
}
BENCHMARK( ureact_signals_construction )
    ->Name( FULL_BENCHMARK_NAME( ureact_signals_construction ) );


static void ureact_signal_reaction( benchmark::State& state )
{
    ureact::context ctx;
    auto a = ctx.make_value( 1 );
    auto b = ureact::make_function( a, std::negate<>() );
    assert( b.get() == -1 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( b.get() == -i );
        ++i;
    }
}
BENCHMARK( ureact_signal_reaction )->Name( FULL_BENCHMARK_NAME( ureact_signal_reaction ) );



static void ureact_chained_functions_baseline( benchmark::State& state )
{
    ureact::context ctx;
    auto a = ctx.make_value( 1 );
    auto d = ureact::make_function( a,
        []( int a ) { //
            return ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );
        } );

    assert( d.get() == 8 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( d.get() == 8 * i * i );
        benchmark::DoNotOptimize( d.get() );
        ++i;
    }
}
BENCHMARK( ureact_chained_functions_baseline )
    ->Name( FULL_BENCHMARK_NAME( ureact_chained_functions_baseline ) );

static void ureact_chained_functions_separate( benchmark::State& state )
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
    for( auto _ : state )
    {
        a <<= i;
        assert( d.get() == 8 * i * i );
        benchmark::DoNotOptimize( d.get() );
        ++i;
    }
}
BENCHMARK( ureact_chained_functions_separate )
    ->Name( FULL_BENCHMARK_NAME( ureact_chained_functions_separate ) );

static void ureact_chained_functions_optimized( benchmark::State& state )
{
    ureact::context ctx;
    auto a = ctx.make_value( 1 );
    auto d = ( ( a + a ) * ( a + a ) ) + ( ( a + a ) * ( a + a ) );

    assert( d.get() == 8 );

    int i = 0;
    for( auto _ : state )
    {
        a <<= i;
        assert( d.get() == 8 * i * i );
        benchmark::DoNotOptimize( d.get() );
        ++i;
    }
}
BENCHMARK( ureact_chained_functions_optimized )
    ->Name( FULL_BENCHMARK_NAME( ureact_chained_functions_optimized ) );
