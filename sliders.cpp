#include "sliders.hpp"

#include "bishop.hpp"
#include "rook.hpp"

/* ------------------------------- rook ------------------------------- */
// // returns squares that could be obstacles in rook's path, so edge squares are not considered
// U64 mask_rook_attacks(int square)
// {
//     // result attacks bitboard for rook
//     U64 attacks = 0ULL;

//     // target rank and files
//     int tr = square / 8;
//     int tf = square % 8;

//     // mask relevant rook occupancy bits
//     for (int r = tr + 1; r <= 6; ++r) attacks |= (1ULL << (r * 8 + tf));
//     for (int r = tr - 1; r >= 1; --r) attacks |= (1ULL << (r * 8 + tf));
//     for (int f = tf + 1; f <= 6; ++f) attacks |= (1ULL << (tr * 8 + f));
//     for (int f = tf - 1; f >= 1; --f) attacks |= (1ULL << (tr * 8 + f));

//     return attacks;
// }

// // generate rook attacks given square and occupancies, used to fill attack table
// U64 generate_rook_attacks(int square, U64 block)
// {
//     // result attacks bitboard for rook
//     U64 attacks = 0ULL;

//     // target rank and files
//     int tr = square / 8;
//     int tf = square % 8;

//     // mask relevant rook occupancy bits
//     for (int r = tr + 1; r <= 7; ++r)        // down
//     {
//          attacks |= (1ULL << (r * 8 + tf));
//          if((1ULL << (r * 8 + tf)) & block) break;
//     }
//     for (int r = tr - 1; r >= 0; --r)        // up
//     {
//          attacks |= (1ULL << (r * 8 + tf));
//          if((1ULL << (r * 8 + tf)) & block) break;
//     }
//     for (int f = tf + 1; f <= 7; ++f)        // right
//     {
//          attacks |= (1ULL << (tr * 8 + f));
//          if((1ULL << (tr * 8 + f)) & block) break;
//     }
//     for (int f = tf - 1; f >= 0; --f)        // left
//     {
//          attacks |= (1ULL << (tr * 8 + f));
//          if((1ULL << (tr * 8 + f)) & block) break;
//     }

//     return attacks;
// }

// // fill table of rook attacks
// void init_rook_attacks()
// {
//     for (int square = 0; square < 64; ++square) {
//         U64 attack_mask = mask_rook_attacks(square);
//         int bits_count = count_bits(attack_mask);
//         int occupancy_idxs = (1 << bits_count);
//         rook_masks[square] = attack_mask;

//         for (int index = 0; index < occupancy_idxs; ++index) {
//             U64 occupancy = set_occupancy(index, bits_count, attack_mask);
//             int magic_idx = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
//             rook_attacks[square][magic_idx] = generate_rook_attacks(square, occupancy);
//         }
//     }
// }

// // get rook attacks using magic bitboard
// U64 get_rook_attacks(int square, U64 occupancy)
// {
//     // get bishop attacks assuming current board occupancy
//     occupancy &= rook_masks[square];
//     occupancy *= rook_magic_numbers[square];
//     occupancy >>= 64 - rook_relevant_bits[square];

//     // return rook attacks
//     return rook_attacks[square][occupancy];
// }

/* ------------------------------- queen ------------------------------- */
// get queen attacks
U64 get_queen_attacks(int square, U64 occupancy)
{
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}