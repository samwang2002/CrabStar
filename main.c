#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#include "board.h"
#include "uci.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perft.h"
#include "timecontrol.h"
#include "hash.h"
#include "net.h"
#include "search.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();
    init_weights();

    // parse_fen(cmk_position);
    // print_board();
    // // printf("%d\n", net_eval());

    // // side = black;
    // // printf("%d\n", net_eval());

    // // clock_t start, end;
    // // start = clock();
    // // for (int i = 0; i < 100000; ++i) evaluate();
    // // end = clock();
    // // printf("regular eval time: %lfs\n", ((double)(end-start)) / CLOCKS_PER_SEC);

    // // start = clock();
    // // for (int i = 0; i < 100000; ++i) net_eval();
    // // end = clock();
    // // printf("network eval time: %lfs\n", ((double)(end-start)) / CLOCKS_PER_SEC);

    // clock_t start, end;
    // start = clock();
    // search_position(10);
    // end = clock();
    // printf("time: %lfs\n", ((double)(end-start)) / CLOCKS_PER_SEC);

    int debug = 0;
    if (debug) {

    } else
         uci_loop();
    return 0;
}