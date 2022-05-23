#include "pawn.h"

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{

    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white panws
    if (!side)
    {
        if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    
    //black panws
    else
    {
        if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }
    return attacks;
}

// init leaper pieces attacks
void init_leapers_attacks()
{
    // loop over 64 board squres
    for (int square = 0; square < 64; square++)
    {
        // inititialize pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
    }

}