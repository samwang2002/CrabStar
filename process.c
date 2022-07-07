#include "process.h"
#include "uci.h"
#include "move.h"
#include "board.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void process_data(const char *pos_url, const char *eval_url, const char *out_url)
{
    srand(time(NULL));

    // open files
    FILE *positions, *evaluations, *output;
    if ((positions = fopen(pos_url, "r")) == NULL) return;
    if ((evaluations = fopen(eval_url, "r")) == NULL) return;
    if ((output = fopen(out_url, "w")) == NULL) return;

    char pos_buf[128], eval_buf[128];

    int position_count = 0;

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

            // 1/5 chance of writing position to output
            if (rand()%5 == 0) {
                write_to_output(output, eval);
                ++position_count;
            }
        }
    }

    fclose(positions);
    fclose(evaluations);
    fclose(output);

    printf("%d positions written to %s\n", position_count, out_url);
}

// writes current position and evaluation to output file
void write_to_output(FILE *fp, int evaluation)
{
    // fwrite(encode_position(), sizeof(char), input_nodes, fp);
    char *encoding = encode_position();
    fputs(encoding, fp);
    free(encoding);
    fprintf(fp, " %d\n", evaluation);
}