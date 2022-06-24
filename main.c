#include <stdio.h>
#include <string.h>
#ifdef WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

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

int get_time_ms()
{
    #ifdef WIN64
        return GetTickCount();
    #else
        struct timeval time_value;
        gettimeofday(&time_value, NULL);
        return time_value.tv_sec * 1000 + time_value.tv_usec / 1000;
    #endif
}

int main()
{
    init_attacks();
    // parse_fen(tricky_position);     // r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
    parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N21qp/PPPBBPpP/R3K2R w KQkq - 0 1");
    print_board();

    move_list moves;
    moves.count = 0;
    generate_moves(&moves, side);

    int start = get_time_ms();

    for (int i = 0; i < moves.count; ++i) {
        int move = moves.moves[i];
        copy_board();
        print_move(move);
        if (!make_move(move, all_moves)) printf("unable to make move\n");
        print_board();
        getchar();
        take_back();
    }

    printf("time taken to execute: %d\n", get_time_ms() - start);

    return 0;
}