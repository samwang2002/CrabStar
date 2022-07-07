#include "process.h"
#include "uci.h"
#include "move.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_data(const char *pos_url, const char *eval_url)
{
    FILE *positions = fopen(pos_url, "r");
    if (positions == NULL) return;
    FILE *evaluations = fopen(eval_url, "r");
    if (evaluations == NULL) return;
    char pos_buf[128], eval_buf[128];

    // skip evaluation file header
    fgets(eval_buf, sizeof(eval_buf), evaluations);

    // loop over games
    for (int idx = 0; idx < 50000; ++idx) {
        // skip over game headers
        do fgets(pos_buf, sizeof(pos_buf), positions);
        while (strncmp(pos_buf, "\n", 1) != 0);

        // skip event counter
        while (fgetc(evaluations) != ',') ;

        // loop through moves
        parse_fen(start_position);
        int c;
        while (1) {
            // get move string
            int char_count = 0;
            while ((c = fgetc(positions)) != EOF && c != ' ') {
                pos_buf[char_count] = c;
                ++char_count;
            }
            pos_buf[char_count] = '\0';
            if (strchr(pos_buf, '-') != NULL) break;
            
            // make move
            int move = parse_move(pos_buf);
            make_move(move, all_moves);

            // get position evaluation
            char_count = 0;
            while ((c = fgetc(evaluations)) != EOF && c != ' ' && c != '\n') {
                eval_buf[char_count] = c;
                ++char_count;
            }
            eval_buf[char_count] = '\0';
            int eval = atoi(eval_buf);

            // print results
            if (idx > 49990) {
                print_move(move);
                printf(", %d cp\n", eval);
            }
        }

        if (idx > 49990) printf("---------------------------------------\n");
    }
}