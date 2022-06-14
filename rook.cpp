#include "rook.hpp"


// mask rook attacks
U64 mask_rook_attacks(int square)
{
    // result attacks bitboard for rook
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (int r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (int r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (int f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (int f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

    return attacks;
}