#include "king.hpp"

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