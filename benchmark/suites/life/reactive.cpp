#include <benchmark/benchmark.h>
#include <reactive/non_thread_safe/ObservableProperty.h>
#include <reactive/non_thread_safe/ReactiveProperty.h>

#include "macros.hpp"
#include "stdfx.hpp"

namespace
{

using namespace reactive::non_thread_safe;

class GameBoard
{
public:
    GameBoard( int width, int height, const std::vector<bool>& values )
        : m_width( width )
        , m_height( height )
    {
        const int fields = width * height;
        assert( values.size() == size_t( fields ) );

        m_oldBoard.reserve( fields );
        for( int i = 0; i < fields; ++i )
        {
            m_oldBoard.emplace_back( values[i] );
        }

        auto oldBoardFieldByPos = [&]( std::pair<int, int> pos ) -> ObservableProperty<bool>& {
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

        m_newBoard.reserve( fields );
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
            m_newBoard.emplace_back( false ).set( updateField, self, tl, t, tr, l, r, bl, b, br );
            // clang-format on
        }
    }

    void update()
    {
        m_recalculated = 0;

        const int fields = m_width * m_height;

        std::vector<bool> tempBoard;
        tempBoard.resize( fields );
        for( int i = 0; i < fields; ++i )
        {
            tempBoard[i] = m_newBoard[i].getCopy();
        }

        for( int i = 0; i < fields; ++i )
        {
            m_oldBoard[i] = bool( tempBoard[i] );
        }
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

    int m_width{};
    int m_height{};
    std::vector<ObservableProperty<bool>> m_oldBoard{};
    std::vector<ReactiveProperty<bool>> m_newBoard{};
    int m_recalculated = -1;
};


void reactive_board_construction( benchmark::State& state )
{
    for( auto it : state )
    {
        GameBoard board(
            board::INITIAL_BOARD_WIDTH, board::INITIAL_BOARD_HEIGHT, board::INITIAL_BOARD_CONFIG );
        benchmark::DoNotOptimize( board );
    }
}
BENCHMARK( reactive_board_construction )
    ->Name( FULL_BENCHMARK_NAME( reactive_board_construction ) );


void reactive_emulation( benchmark::State& state )
{
    for( auto it : state )
    {
        GameBoard board(
            board::INITIAL_BOARD_WIDTH, board::INITIAL_BOARD_HEIGHT, board::INITIAL_BOARD_CONFIG );

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
BENCHMARK( reactive_emulation )->Name( FULL_BENCHMARK_NAME( reactive_emulation ) );

} // namespace
