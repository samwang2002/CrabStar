#include "king.h"
#include "constants.h"

U64 king_attacks[64];

// generate king attacks
U64 mask_king_attacks(const int square)
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

// initialize king attack table
void init_king_attacks()
{
    for (int square = 0; square <= 64; ++square)
        king_attacks[square] = mask_king_attacks(square);
}

// generate castling moves
void generate_castling_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies,
                             const int castle, int (* const attacked)(int, const int))
{
    if (side == white) {
        if ((castle & wk) && !get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1)
            && !attacked(e1, black) && !attacked(f1, black) && !attacked(g1, black))
                add_move(moves, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
        if ((castle & wq) && !get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1)
            && !get_bit(occupancies[both], b1) && !attacked(e1, black)
            && !attacked(d1, black) && !attacked(c1, black))
                add_move(moves, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
    } else {
        if ((castle & bk) && !get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8)
            && !attacked(e8, white) && !attacked(f8, white) && !attacked(g8, white))
            add_move(moves, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
        if ((castle & bq) && !get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8)
            && !get_bit(occupancies[both], b8) && !attacked(e8, white) && !attacked(d8, white)
            && !attacked(c8, white))
                add_move(moves, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
    }
}

// generate other king moves
void generate_king_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies)
{
    if (side == white) {
        U64 bitboard = bitboards[K];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = king_attacks[source] & ~occupancies[white];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[black], target))       // quiet move
                    add_move(moves, encode_move(source, target, K, 0, 0, 0, 0, 0));
                else                                            // capture
                    add_move(moves, encode_move(source, target, K, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[k];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = king_attacks[source] & ~occupancies[black];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[white], target))       // quiet move
                    add_move(moves, encode_move(source, target, k, 0, 0, 0, 0, 0));
                else                                            // capture
                    add_move(moves, encode_move(source, target, k, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    }
}