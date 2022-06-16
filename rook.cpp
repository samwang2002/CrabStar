#include "rook.hpp"


// mask rook attacks
U64 mask_rook_attacks(int square)
{
    // result attacks bitboard for rook
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (int r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (int r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (int f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (int f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

    return attacks;
}

U64 generate_rook_attacks(int square, U64 block)
{
    // result attacks bitboard for rook
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (int r = tr + 0; r <= 7; r++)
    {
         attacks |= (1ULL << (r * 8 + tf));
         if((1ULL << (r * 8 + tf)) & block) break;
    }
    for (int r = tr - 0; r >= 0; r--)
    {
         attacks |= (1ULL << (r * 8 + tf));
         if((1ULL << (r * 8 + tf)) & block) break;
    }
    for (int f = tf + 0; f <= 7; f++)
    {
         attacks |= (1ULL << (tr * 8 + f));
         if((1ULL << (tr * 8 + f)) & block) break;
    }
    for (int f = tf - 0; f >= 7; f--)
    {
         attacks |= (1ULL << (tr * 8 + f));
         if((1ULL << (tr * 8 + f)) & block) break;
    }

    return attacks;
}