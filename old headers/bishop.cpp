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

U64 generate_bishop_attacks(int square, U64 block)
{
     U64 attacks = 0ULL;

    // target rank and files
     int tr = square / 8;
     int tf = square % 8;

     // mask relevant bishop occupancy bits
     for (int r = tr+1, f=tf+1; r <= 7 && f <= 7; r++, f++)     // bottom right
     {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f) & block)) break;
     }
     for (int r = tr-1, f=tf+1; r >= 0 && f <= 7; r--, f++)     // top right
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f) & block)) break;
     }
     for (int r = tr+1, f=tf-1; r <= 7 && f >= 0; r++, f--)     // bottom left
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f)) & block) break;
     }
     for (int r = tr-1, f=tf-1; r >= 0 && f >= 0; r--, f--)     // top left
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f)) & block) break;
     }
     
     return attacks;
}

// fill table of bishop attacks
void init_bishop_attacks()
{
    for (int square = 0; square < 64; ++square) {
        U64 attack_mask = mask_bishop_attacks(square);
        int bits_count = count_bits(attack_mask);
        int occupancy_idxs = (1 << bits_count);
        bishop_masks[square] = attack_mask;

        for (int index = 0; index < occupancy_idxs; ++index) {
            U64 occupancy = set_occupancy(index, bits_count, attack_mask);
            int magic_idx = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
            bishop_attacks[square][magic_idx] = generate_bishop_attacks(square, occupancy);
        }
    }
}

U64 get_bishop_attacks(int square, U64 occupancy)
{
    // get bishop attacks assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    // return bishop attacks
    return bishop_attacks[square][occupancy];
}