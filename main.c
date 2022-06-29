#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "board.h"
#include "uci.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perft.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_attacks();

    int debug = 1;
    if (debug) {
        parse_fen(tricky_position);
        print_board();
        #ifndef WIN_64
            setlocale(LC_NUMERIC, "");
        #endif
        search_position(5);
        // print_move_list(&moves);
    } else
        uci_loop();

    return 0;
}