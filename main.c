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
        search_position(3);

        // move_list moves;
        // generate_moves(&moves);
        // killer_moves[0][ply] = moves.moves[0];
        // killer_moves[1][ply] = moves.moves[2];
        // history_moves[P][a4] = 30;
        // sort_moves(&moves);

        // for (int i = 0; i < moves.count; ++i) {
        //     print_move(moves.moves[i]);
        //     printf(": %d\n", score_move(moves.moves[i]));
        // }
    } else
        uci_loop();

    return 0;
}