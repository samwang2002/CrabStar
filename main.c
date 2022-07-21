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

    clock_t start, end;
    start = clock();

    simulate_generations(1, 8, 3, 2, "seed1", 5, 0.05);

    end = clock();
    printf("-------------------\ntime taken: %0.2fs\n", (double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
