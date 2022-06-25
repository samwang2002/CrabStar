#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perf.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_attacks();
    // parse_fen(tricky_position);     // r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
    // parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N21qp/PPPBBPpP/R3K2R w KQkq - 0 1");
    // parse_fen(start_position);
    // parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    parse_fen("8/8/8/8/8/k7/7q/K7 w - -");
    print_board();


    // int start = get_time_ms();

    // perft
    setlocale(LC_ALL, "");
    printf("%d has legal moves: %d\n", side, has_legal_moves(side));
    // side = white;
    // perft_test(6);
    print_board();
    // print_bitboard(bitboards[N]);
    // getchar();

    return 0;
}