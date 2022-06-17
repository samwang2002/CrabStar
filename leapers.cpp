#include "leapers.hpp"

// defining global variables
U64 king_attacks[64];
U64 knight_attacks[64];
U64 pawn_attacks[2][64];


/* ------------------------------- king ------------------------------- */
// generate king attacks
U64 mask_king_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate king attacks
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);       // right
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);       // top left
    if  (bitboard << 8)               attacks |= (bitboard << 8);       // top
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);       // top right

    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);       // left
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);       // bottom right
    if  (bitboard >> 8)               attacks |= (bitboard >> 8);       // bottom
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);       // bottom left

    return attacks;
}

/* ------------------------------- knight ------------------------------- */
// generate knight attacks
U64 mask_knight_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    //generate knight attacks 17, 15, 10 ,6
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    // return attack map
    return attacks;
}

/* ------------------------------- pawn ------------------------------- */
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