#include "rook.h"
#include "constants.h"

// returns squares that could be obstacles in rook's path, so edge squares are not considered
U64 mask_rook_attacks(const int square)
{
    // result attacks bitboard for rook
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (int r = tr + 1; r <= 6; ++r) attacks |= (1ULL << (r * 8 + tf));
    for (int r = tr - 1; r >= 1; --r) attacks |= (1ULL << (r * 8 + tf));
    for (int f = tf + 1; f <= 6; ++f) attacks |= (1ULL << (tr * 8 + f));
    for (int f = tf - 1; f >= 1; --f) attacks |= (1ULL << (tr * 8 + f));

    return attacks;
}

// generate rook attacks given square and occupancies, used to fill attack table
U64 generate_rook_attacks(const int square, const U64 block)
{
    // result attacks bitboard for rook
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant rook occupancy bits
    for (int r = tr + 1; r <= 7; ++r)        // down
    {
         attacks |= (1ULL << (r * 8 + tf));
         if((1ULL << (r * 8 + tf)) & block) break;
    }
    for (int r = tr - 1; r >= 0; --r)        // up
    {
         attacks |= (1ULL << (r * 8 + tf));
         if((1ULL << (r * 8 + tf)) & block) break;
    }
    for (int f = tf + 1; f <= 7; ++f)        // right
    {
         attacks |= (1ULL << (tr * 8 + f));
         if((1ULL << (tr * 8 + f)) & block) break;
    }
    for (int f = tf - 1; f >= 0; --f)        // left
    {
         attacks |= (1ULL << (tr * 8 + f));
         if((1ULL << (tr * 8 + f)) & block) break;
    }

    return attacks;
}

// fill table of rook attacks
void init_rook_attacks()
{
    for (int square = 0; square < 64; ++square) {
        U64 attack_mask = mask_rook_attacks(square);
        int bits_count = count_bits(attack_mask);
        int occupancy_idxs = (1 << bits_count);
        rook_masks[square] = attack_mask;

        for (int index = 0; index < occupancy_idxs; ++index) {
            U64 occupancy = set_occupancy(index, bits_count, attack_mask);
            int magic_idx = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
            rook_attacks[square][magic_idx] = generate_rook_attacks(square, occupancy);
        }
    }
}

// get rook attacks using magic bitboard
U64 get_rook_attacks(const int square, const U64 occupancy)
{
    return rook_attacks[square][((occupancy & rook_masks[square]) * rook_magic_numbers[square])
                                >> (64 - rook_relevant_bits[square])];
}

// generate rook moves
void generate_rook_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies)
{
    if (side == white) {
        U64 bitboard = bitboards[R];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = get_rook_attacks(source, occupancies[both]) & ~occupancies[white];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[black], target))       // quiet move
                    add_move(moves, encode_move(source, target, R, 0, 0, 0, 0, 0));
                else                                            // capture move
                    add_move(moves, encode_move(source, target, R, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[r];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = get_rook_attacks(source, occupancies[both]) & ~occupancies[black];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[white], target))
                    add_move(moves, encode_move(source, target, r, 0, 0, 0, 0, 0));
                else
                    add_move(moves, encode_move(source, target, r, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    }
}
