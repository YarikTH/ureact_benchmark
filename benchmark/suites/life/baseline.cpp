#include <benchmark/benchmark.h>

#include "macros.hpp"

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
            m_oldBoard.push_back( values[i] );
        }

        m_newBoard = m_oldBoard;
    }

    void update()
    {
        m_oldBoard = m_newBoard;

        auto oldBoardFieldByPos = [&]( std::pair<int, int> pos ) -> bool {
            const auto posWrapped = wrapPos( pos );
            const int i = posToFieldId( posWrapped );
            return m_oldBoard[i];
        };

        // clang-format off
        auto updateField = []( bool self, bool tl, bool t, bool tr, bool l, bool r, bool bl, bool b, bool br )
        {
            // clang-format on
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

        const int fields = m_width * m_height;
        for( int i = 0; i < fields; ++i )
        {
            // calculate new board using values form old board
            // clang-format off
            const auto self_pos = fieldIdToPos( i );
            const bool self = oldBoardFieldByPos( self_pos );
            const bool tl   = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second - 1 } );
            const bool t    = oldBoardFieldByPos( { self_pos.first,     self_pos.second - 1 } );
            const bool tr   = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second - 1 } );
            const bool l    = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second     } );
            const bool r    = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second     } );
            const bool bl   = oldBoardFieldByPos( { self_pos.first - 1, self_pos.second + 1 } );
            const bool b    = oldBoardFieldByPos( { self_pos.first,     self_pos.second + 1 } );
            const bool br   = oldBoardFieldByPos( { self_pos.first + 1, self_pos.second + 1 } );
            m_newBoard[i] = updateField( self, tl, t, tr, l, r, bl, b, br );
            // clang-format on
        }

        m_finished = ( m_newBoard == m_oldBoard );
    }

    [[nodiscard]] bool finished() const
    {
        return m_finished;
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
    std::vector<bool> m_oldBoard{};
    std::vector<bool> m_newBoard{};
    bool m_finished = false;
};

#define _ false
#define O true

// clang-format off
const std::vector<bool> INITIAL_BOARD_CONFIG = {
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,O,_,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,O,O,_,_,_,_,_,O,O,O,_,_,_,_,_,_,_,_,
    _,_,O,_,_,_,_,_,_,_,_,_,_,_,_,_,O,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,O,_,O,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,O,_,O,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,O,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,O,O,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
    _,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
};
// clang-format on

constexpr int WIDTH = 20;
constexpr int HEIGHT = 20;


static void baseline_board_construction( benchmark::State& state )
{
    for( auto it : state )
    {
        GameBoard board( WIDTH, HEIGHT, INITIAL_BOARD_CONFIG );
        benchmark::DoNotOptimize( board );
    }
}
BENCHMARK( baseline_board_construction )
    ->Name( FULL_BENCHMARK_NAME( baseline_board_construction ) );


static void baseline_emulation( benchmark::State& state )
{
    for( auto it : state )
    {
        GameBoard board( WIDTH, HEIGHT, INITIAL_BOARD_CONFIG );

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
BENCHMARK( baseline_emulation )->Name( FULL_BENCHMARK_NAME( baseline_emulation ) );
