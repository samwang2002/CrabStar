#include "queen.h"
#include "constants.h"
#include "bishop.h"
#include "rook.h"

// get queen attacks
U64 get_queen_attacks(int square, U64 occupancy)
{
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}

// generate queen moves
void generate_queen_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies)
{
    if (side == white) {
        U64 bitboard = bitboards[Q];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = get_queen_attacks(source, occupancies[both]) & ~occupancies[white];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[black], target))       // quiet move
                    add_move(moves, encode_move(source, target, Q, 0, 0, 0, 0, 0));
                else                                            // capture move
                    add_move(moves, encode_move(source, target, Q, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[q];
        while (bitboard) {
            int source = ls1b(bitboard);
            U64 attacks = get_queen_attacks(source, occupancies[both]) & ~occupancies[black];
            while (attacks) {
                int target = ls1b(attacks);
                if (!get_bit(occupancies[white], target))
                    add_move(moves, encode_move(source, target, q, 0, 0, 0, 0, 0));
                else
                    add_move(moves, encode_move(source, target, q, 0, 1, 0, 0, 0));
                pop_bit(attacks, target);
            }
            pop_bit(bitboard, source);
        }
    }
}