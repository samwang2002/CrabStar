#include "process.h"
#include "uci.h"
#include "move.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_data(const char *pos_url, const char *evals_url)
{
    FILE *positions = fopen(pos_url, "r");
    if (positions == NULL) return;
    char buf[4096];

    // skip over headers
    do fgets(buf, sizeof(buf), positions);
    while (strncmp(buf, "\n", 1) != 0);

    // loop through moves
    parse_fen(start_position);
    int c;
    while (1) {
        int char_count = 0;
        while ((c = fgetc(positions)) != EOF && c != ' ') {
            buf[char_count] = c;
            ++char_count;
        }
        buf[char_count] = '\0';
        if (strchr(buf, '-') != NULL) break;
        
        int move = parse_move(buf);
        // print_move(move);
        // printf("\n");
        make_move(move, all_moves);
        // print_board();
        // printf("\n");
    }

    // skip over headers
    do fgets(buf, sizeof(buf), positions);
    while (strncmp(buf, "\n", 1) != 0);

    // loop through moves
    parse_fen(start_position);
    while (1) {
        int char_count = 0;
        while ((c = fgetc(positions)) != EOF && c != ' ') {
            buf[char_count] = c;
            ++char_count;
        }
        buf[char_count] = '\0';
        if (strchr(buf, '-') != NULL) break;
        
        int move = parse_move(buf);
        print_move(move);
        printf("\n");
        make_move(move, all_moves);
        print_board();
        printf("\n");
    }
}