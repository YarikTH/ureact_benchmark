#include <benchmark/benchmark.h>

#include "macros.hpp"
#include "stdfx.hpp"

namespace
{

class GameBoard
{
public:
    GameBoard( int width, int height, const std::vector<bool>& values )
        : m_width( width )
        , m_height( height )
    {
        const int fields = width * height;
        assert( values.size() == size_t( fields ) );

        m_oldBoard.resize( fields );
        m_newBoard.resize( fields );
        for( int i = 0; i < fields; ++i )
        {
            m_oldBoard[i] = m_newBoard[i] = values[i];
        }

        m_neighbours.resize( fields );
        for( int i = 0; i < fields; ++i )
        {
            // clang-format off
            const auto self_pos = fieldIdToPos( i );
            m_neighbours[i].tl = posToFieldIdWrapped( { self_pos.first - 1, self_pos.second - 1 } );
            m_neighbours[i].t  = posToFieldIdWrapped( { self_pos.first,     self_pos.second - 1 } );
            m_neighbours[i].tr = posToFieldIdWrapped( { self_pos.first + 1, self_pos.second - 1 } );
            m_neighbours[i].l  = posToFieldIdWrapped( { self_pos.first - 1, self_pos.second     } );
            m_neighbours[i].r  = posToFieldIdWrapped( { self_pos.first + 1, self_pos.second     } );
            m_neighbours[i].bl = posToFieldIdWrapped( { self_pos.first - 1, self_pos.second + 1 } );
            m_neighbours[i].b  = posToFieldIdWrapped( { self_pos.first,     self_pos.second + 1 } );
            m_neighbours[i].br = posToFieldIdWrapped( { self_pos.first + 1, self_pos.second + 1 } );
            // clang-format on
        }
    }

    void update()
    {
        m_oldBoard.swap( m_newBoard );

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
            const auto& neighbours = m_neighbours[i];
            const bool self = m_oldBoard[i];
            const bool tl   = m_oldBoard[neighbours.tl];
            const bool t    = m_oldBoard[neighbours.t];
            const bool tr   = m_oldBoard[neighbours.tr];
            const bool l    = m_oldBoard[neighbours.l];
            const bool r    = m_oldBoard[neighbours.r];
            const bool bl   = m_oldBoard[neighbours.bl];
            const bool b    = m_oldBoard[neighbours.b];
            const bool br   = m_oldBoard[neighbours.br];
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

    [[nodiscard]] int posToFieldIdWrapped( std::pair<int, int> pos ) const
    {
        return posToFieldId( wrapPos( pos ) );
    }

    struct Neighbours
    {
        int tl, t, tr, l, r, bl, b, br;
    };

    int m_width{};
    int m_height{};
    std::vector<bool> m_oldBoard{};
    std::vector<bool> m_newBoard{};
    std::vector<Neighbours> m_neighbours{};
    bool m_finished = false;
};

void baseline_board_construction( benchmark::State& state )
{
    for( auto it : state )
    {
        GameBoard board(
            board::INITIAL_BOARD_WIDTH, board::INITIAL_BOARD_HEIGHT, board::INITIAL_BOARD_CONFIG );
        benchmark::DoNotOptimize( board );
    }
}
BENCHMARK( baseline_board_construction )
    ->Name( FULL_BENCHMARK_NAME( baseline_board_construction ) );


void baseline_emulation( benchmark::State& state )
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
BENCHMARK( baseline_emulation )->Name( FULL_BENCHMARK_NAME( baseline_emulation ) );

} // namespace
