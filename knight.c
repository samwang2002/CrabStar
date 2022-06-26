#include "knight.h"
#include "constants.h"

U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(const int square)
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

// initialize knight attack table
void init_knight_attacks()
{
    for (int square = 0; square <= 64; ++square)
        knight_attacks[square] = mask_knight_attacks(square);
}

// generate knight moves
void generate_knight_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies)
{
    if (side == white) {
        U64 bitboard = bitboards[N];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = knight_attacks[source] & ~occupancies[white];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[black], target))       // quiet move
                    add_move(moves, encode_move(source, target, N, 0, 0, 0, 0, 0));
                else
                    add_move(moves, encode_move(source, target, N, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[n];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = knight_attacks[source] & ~occupancies[black];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[white], target))       // quiet move
                    add_move(moves, encode_move(source, target, n, 0, 0, 0, 0, 0));
                else
                    add_move(moves, encode_move(source, target, n, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    }
}