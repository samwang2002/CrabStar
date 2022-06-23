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
    parse_fen("r3k2r/pP1pqpb1/bn2pnp1/2pPN3/Pp2P3/2N2Q1p/PPPBBPpP/R3K2R b KQkq a3 0 1 ");
    print_board();

    move_list moves;
    moves.count = 0;
    // int color = white;
    generate_moves(&moves, side);
    print_move_list(&moves);

    for (int i = 0; i < moves.count; ++i) {
        int move = moves.moves[i];
        copy_board();
        make_move(move, all_moves);
        print_board();
        getchar();
        take_back();
    }

    return 0;
}