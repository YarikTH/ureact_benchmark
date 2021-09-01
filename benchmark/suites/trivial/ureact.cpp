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
