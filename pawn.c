#include "pawn.h"
#include "constants.h"

U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // white pawns
    if (!side)
    {
        if((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    
    // black pawns
    else
    {
        if((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }
    return attacks;
}

// initialize pawn attack table
void init_pawn_attacks()
{
    for (int square = 0; square <= 64; ++square) {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
    }
}

// generate pawn moves
void generate_pawn_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies, int enpassant)
{
    if (side == white) {
        U64 bitboard = bitboards[P];
        while (bitboard) {
            int source = get_ls1b_index(bitboard);

            // quiet moves
            int target = source - 8;
            if (!get_bit(occupancies[both], target)) {      // target square is not occupied
                if (source <= h7) {      // promotions
                    add_move(moves, encode_move(source, target, P, Q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, R, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, B, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, N, 0, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, P, 0, 0, 0, 0, 0));     // single push
                    if (source >= a2 && !get_bit(occupancies[both], source-16))         // double push
                        add_move(moves, encode_move(source, source-16, P, 0, 0, 1, 0, 0));
                }
            }

            // regular captures
            U64 attacks = pawn_attacks[white][source] & occupancies[black];
            while (attacks) {
                target = get_ls1b_index(attacks);
                if (source <= h7) {     // promotions
                    add_move(moves, encode_move(source, target, P, Q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, R, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, B, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, N, 1, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, P, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target);
            }

            // en passant captures
            if (enpassant != no_sq) {
                U64 enpas_attacks = pawn_attacks[white][source];
                if (enpas_attacks & (1ULL << enpassant)) {
                    target = get_ls1b_index(enpas_attacks);
                    add_move(moves, encode_move(source, target, P, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[p];
        while (bitboard) {
            int source = get_ls1b_index(bitboard);

            // quiet moves
            int target = source + 8;
            if (!get_bit(occupancies[both], target)) {
                if (source >= a2) {
                    add_move(moves, encode_move(source, target, p, q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, r, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, b, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, n, 0, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, p, 0, 0, 0, 0, 0));
                    if (source <= h7 && !get_bit(occupancies[both], source+16))
                        add_move(moves, encode_move(source, source+16, p, 0, 0, 1, 0, 0));
                }
            }

            // regular captures
            U64 attacks = pawn_attacks[black][source] & occupancies[white];
            while (attacks) {
                target = get_ls1b_index(attacks);
                if (source >= a2) {     // promotions
                    add_move(moves, encode_move(source, target, p, q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, r, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, b, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, n, 1, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, p, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target);
            }

            // en passant captures
            if (enpassant != no_sq) {
                U64 enpas_attacks = pawn_attacks[black][source];
                if (enpas_attacks & (1ULL << enpassant)) {
                    target = get_ls1b_index(enpas_attacks);
                    add_move(moves, encode_move(source, target, p, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, source);
        }
    }
}