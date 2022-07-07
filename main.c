#include <stdio.h>
#include <string.h>
#include <locale.h>

#include "board.h"
#include "uci.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perft.h"
#include "timecontrol.h"
#include "hash.h"
#include "process.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();
    process_data("data_uci.pgn", "stockfish.csv", "training.txt");

    // parse_fen(cmk_position);
    // // side = black;
    // print_board();
    // char *encoding = encode_position();

    // // print encoding as bitboards
    // for (int count = 0; count < 16; ++count) {
    //     for (int i = 0; i < 8; ++i) {
    //         for (int j = 0; j < 8; ++j) {
    //             if (encoding[count*64 + i*8 + j])
    //                 printf("%d ", encoding[count*64 + i*8 + j]);
    //             else
    //                 printf(". ");
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }

    return 0;
}