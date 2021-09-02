#include <benchmark/benchmark.h>
//#define UREACT_USE_STD_ALGORITHM
#include <ureact/ureact.hpp>

#include "macros.hpp"
#include "stdfx.hpp"

namespace
{

class GameBoard
{
public:
    GameBoard( ureact::context& ctx, int width, int height, const std::vector<bool>& values )
        : m_ctx( ctx )
        , m_width( width )
        , m_height( height )
    {
        const int fields = width * height;
        assert( values.size() == size_t( fields ) );

        m_oldBoard.reserve( fields );
        for( int i = 0; i < fields; ++i )
        {
            m_oldBoard.push_back( ctx.make_value( values[i] ) );
        }

        auto oldBoardFieldByPos = [&]( std::pair<int, int> pos ) -> ureact::value<bool>& {
            const auto posWrapped = wrapPos( pos );
            const int i = posToFieldId( posWrapped );
            return m_oldBoard[i];
        };

        // clang-format off
        auto updateField = [this]( bool self, bool tl, bool t, bool tr, bool l, bool r, bool bl, bool b, bool br )
        {
            // clang-format on
            ++m_recalculated;
            const int neighbours = tl + t + tr + l + r + bl + b + br;
            if( self )
            {
                // If a cell is ON and has only 1 neighbour then it turns OFF in the next turn. (SOLITUDE)
                // If a cell is ON and has 2 or 3 neighbours then it remains ON in the next turn.
                // If a cell is ON and has 4 or more neighbours then it turns OFF in the next turn. (OVERPOPULATION)
                return neighbours == 2 || neighbours == 3;
            }
            else
            {
                // If a cell is OFF and has exactly 3 neighbours then it turns ON in the next turn. (REPRODUCTION)
                return neighbours == 3;
            }
        };

        m_newBoard.resize( fields );
        for( int i = 0; i < fields; ++i )
        {
            // clang-format off
            auto self_pos = fieldIdToPos( i );
            auto& self = oldBoardFieldByPos( self_pos );
            auto& tl   = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second - 1 } );
            auto& t    = oldBoardFieldByPos( { self_pos.first,     self_pos.second - 1 } );
            auto& tr   = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second - 1 } );
            auto& l    = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second     } );
            auto& r    = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second     } );
            auto& bl   = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second + 1 } );
            auto& b    = oldBoardFieldByPos( { self_pos.first,     self_pos.second + 1 } );
            auto& br   = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second + 1 } );
            m_newBoard[i] = with(self, tl, t, tr, l, r, bl, b, br) | updateField;
            // clang-format on
        }
    }

    void update()
    {
        m_recalculated = 0;
        m_ctx.do_transaction( [&]() {
            const int fields = m_width * m_height;
            for( int i = 0; i < fields; ++i )
            {
                m_oldBoard[i] <<= m_newBoard[i].get();
            }
        } );
    }

    [[nodiscard]] int recalculated() const
    {
        return m_recalculated;
    }

    [[nodiscard]] bool finished() const
    {
        return m_recalculated == 0;
    }

private:
    [[nodiscard]] std::pair<int, int> fieldIdToPos( const int fieldId ) const
    {
        return { fieldId % m_width, fieldId / m_width };
    }

    [[nodiscard]] int posToFieldId( const std::pair<int, int> pos ) const
    {
        return pos.first + pos.second * m_width;
    }

    std::pair<int, int>& wrapPos( std::pair<int, int>& pos ) const
    {
        pos.first = ( pos.first + m_width ) % m_width;
        pos.second = ( pos.second + m_height ) % m_height;
        return pos;
    }

    ureact::context& m_ctx;
    int m_width{};
    int m_height{};
    std::vector<ureact::value<bool>> m_oldBoard{};
    std::vector<ureact::function<bool>> m_newBoard{};
    int m_recalculated = -1;
};


void ureact_std_algo_board_construction( benchmark::State& state )
{
    for( auto it : state )
    {
        ureact::context ctx;

        GameBoard board( ctx,
            board::INITIAL_BOARD_WIDTH,
            board::INITIAL_BOARD_HEIGHT,
            board::INITIAL_BOARD_CONFIG );
        benchmark::DoNotOptimize( board );
    }
}
BENCHMARK( ureact_std_algo_board_construction )
    ->Name( FULL_BENCHMARK_NAME( ureact_std_algo_board_construction ) );


void ureact_std_algo_emulation( benchmark::State& state )
{
    for( auto it : state )
    {
        ureact::context ctx;

        GameBoard board( ctx,
            board::INITIAL_BOARD_WIDTH,
            board::INITIAL_BOARD_HEIGHT,
            board::INITIAL_BOARD_CONFIG );

        bool skipUpdate = true;

        int loops = 0;
        do
        {
            if( !skipUpdate )
            {
                board.update();
            }
            skipUpdate = false;
            ++loops;
        } while( !board.finished() );
        assert( loops == 602 );
    }
}
BENCHMARK( ureact_std_algo_emulation )->Name( FULL_BENCHMARK_NAME( ureact_std_algo_emulation ) );

} // namespace
