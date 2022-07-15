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
    net_weights nw;
    printf("%d\n", sizeof(nw));
    read_weights(&nw, "model");

    parse_fen(tricky_position);
    print_board();

    clock_t start, end;
    start = clock();
    negamax(-infinity, infinity, 5, &nw);
    end = clock();
    printf("time: %lfs\n", ((double)(end-start)) / CLOCKS_PER_SEC);

    int debug = 1;
    if (debug) {

    } else
         uci_loop();
    return 0;
}