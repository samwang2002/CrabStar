#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <pthread.h>

#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "constants.h"
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

    time_t start_s, end_s;
    clock_t start, end;
    time(&start_s);
    start = clock();

    simulate_generations(30, 16, 3, "seed1", 5, 0.03);

    // net_weights player1, player2, player3, player4, player5, player6, player7, player8, player9, player10;
    // read_weights(&player1, "seed1");
    // read_weights(&player2, "gen10");
    // read_weights(&player3, "gen20");
    // read_weights(&player4, "gen30");
    // read_weights(&player5, "gen40");
    // read_weights(&player6, "gen50");
    // read_weights(&player7, "gen60");
    // read_weights(&player8, "gen70");
    // read_weights(&player9, "gen80");
    // read_weights(&player10, "gen90");
    // net_weights *players[] = {&player1, &player2, &player3, &player4, &player5, &player6, &player7, &player8,
    //                             &player9, &player10};

    // int elos[10];
    // memset(elos, 0, sizeof(elos));
    // tournament(players, 5, 3, elos);
    // for (int i = 0; i < 10; ++i)
    //     printf("%d: %d\n", i, elos[i]);


    time(&end_s);
    end = clock();
    printf("%s\ntime elapsed: %lds\n", horizontal_line, end_s-start_s);
    printf("CPU time: %lus\n", (int)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
