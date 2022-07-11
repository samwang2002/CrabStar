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
#include "net.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();
    float weights1[nodes2*nodes3];
    load_weights(weights1, nodes2*nodes3, "model/layer2-weights.txt");
    // parse_fen("6k1/p5p1/8/8/8/8/6PP/6K1 w - - 0 1");
    // print_board();
    // printf("evaluation: %d cp\n", evaluate());

    int debug = 1;
    if (debug) {

    } else
         uci_loop();
    return 0;
}