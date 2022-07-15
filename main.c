#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
#include "perft.h"
#include "timecontrol.h"
#include "hash.h"
#include "net.h"
#include "search.h"
#include "game.h"
#include "random.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

int main()
{
    init_all();
    srand(time(NULL));

    net_weights player1, player2;
    read_weights(&player1, "seed1");
    // read_weights(&player2, "seed2");
    player2 = player1;
    mutate(&player2, 1, 1);

    parse_fen(start_position);
    printf("result: %d\n|", match(&player1, &player2, 3, 1));
    print_board();

    return 0;
}