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
    // init_weights("120");

    int debug = 0;
    if (debug) {
        parse_fen(start_position);
        // parse_fen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
        print_board();

        clock_t start, end;
        start = clock();
        search_position(8);
        end = clock();
        printf("time: %lfs\n", ((double)(end-start)) / CLOCKS_PER_SEC);
    } else
         uci_loop();
    return 0;
}