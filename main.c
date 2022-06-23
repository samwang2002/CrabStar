#include <stdio.h>
#include <string.h>
#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"


int main()
{
    init_attacks();
    // parse_fen(tricky_position);
    parse_fen("r3k2r/pP1pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q11/1PPPBPPP/R3K2R w KQkq - 0 1 ");
    // parse_fen("8/6P1/8/8/8/6pP/6P1/8 w - -");
    print_board();

    move_list moves;
    moves.count = 0;
    int color = black;
    generate_moves(&moves, color);
    // generate_king_moves(&moves, color, bitboards, occupancies);
    // generate_queen_moves(&moves, color, bitboards, occupancies);
    // generate_rook_moves(&moves, color, bitboards, occupancies);
    // generate_bishop_moves(&moves, color, bitboards, occupancies);
    // generate_pawn_moves(&moves, color, bitboards, occupancies, enpassant);
    // generate_knight_moves(&moves, color, bitboards, occupancies);
    // generate_castling_moves(&moves, color, bitboards, occupancies, castle, &square_attacked);
    print_move_list(&moves);

    return 0;
}