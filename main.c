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

    net_weights player1, player2, player3, player4;
    read_weights(&player1, "seed1");
    
    player2 = player1;
    player3 = player1;
    player4 = player1;
    mutate(&player2, 1, 1);
    mutate(&player3, 2, 1);
    mutate(&player4, 3, 0.2);

    int results[4];
    net_weights *players[] = {&player1, &player2, &player3, &player4};
    tournament(&players, 2, 3, results);

    return 0;
}