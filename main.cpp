#include <iostream>
#include "board.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"


int main()
{
    init_all();
    
    // initial position
    set_bit(bitboards[P], a2);
    set_bit(bitboards[P], b2);
    set_bit(bitboards[P], c2);
    set_bit(bitboards[P], d2);
    set_bit(bitboards[P], e2);
    set_bit(bitboards[P], f2);
    set_bit(bitboards[P], g2);
    set_bit(bitboards[P], h2);

    set_bit(bitboards[R], a1);
    set_bit(bitboards[R], h1);

    set_bit(bitboards[N], b1);
    set_bit(bitboards[N], g1);
    
    set_bit(bitboards[B], c1);
    set_bit(bitboards[B], f1);

    set_bit(bitboards[Q], d1);
    set_bit(bitboards[K], e1);

    set_bit(bitboards[p], a7);
    set_bit(bitboards[p], b7);
    set_bit(bitboards[p], c7);
    set_bit(bitboards[p], d7);
    set_bit(bitboards[p], e7);
    set_bit(bitboards[p], f7);
    set_bit(bitboards[p], g7);
    set_bit(bitboards[p], h7);

    set_bit(bitboards[r], a8);
    set_bit(bitboards[r], h8);

    set_bit(bitboards[n], b8);
    set_bit(bitboards[n], g8);
    
    set_bit(bitboards[b], c8);
    set_bit(bitboards[b], f8);

    set_bit(bitboards[q], d8);
    set_bit(bitboards[k], e8);

    // test that game states are printed correctly
    side = black;
    enpassant = e3;
    castle |= wk;
    castle |= wq;
    castle |= bk;
    castle |= bq;

    print_board();

    // print all bitboards
    for (int i = 0; i < 12; ++i)
        print_bitboard(bitboards[i]);

    return 0;
}