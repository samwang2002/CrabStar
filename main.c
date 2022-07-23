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

    // simulate_generations(30, 32, 3, "seed1", 5, 0.03);

    int n_players = 4;
    net_weights *players[n_players];
    for (int i = 0; i < n_players; ++i) players[i] = calloc(1, sizeof(net_weights));
    read_weights(players[0], "seed1");
    for (int i = 1; i < n_players; ++i) {
        char dir_path[100];
        sprintf(dir_path, "gen%d", i*10);
        read_weights(players[i], dir_path);
    }
    round_robin(players, n_players, 3, 1);


    time(&end_s);
    end = clock();
    printf("%s\ntime elapsed: %lds\n", horizontal_line, end_s-start_s);
    printf("CPU time: %lus\n", (int)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
