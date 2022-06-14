#include "bishop.hpp"

U64 mask_bishop_attacks(int square)
{
    // returns squares that could be obstacles in bishop's path, so edge squares are not considered
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (int r = tr+1, f=tf+1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r*8 + f));
    for (int r = tr-1, f=tf+1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r*8 + f));
    for (int r = tr+1, f=tf-1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r*8 + f));
    for (int r = tr-1, f=tf-1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r*8 + f));

    return attacks;
}