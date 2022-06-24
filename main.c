#include <stdio.h>
#include <string.h>

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
    parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N21qp/PPPBBPpP/R3K2R w KQkq - 0 1");
    print_board();


    int start = get_time_ms();


    // perft
    perft_test(5);
    getchar();

    return 0;
}