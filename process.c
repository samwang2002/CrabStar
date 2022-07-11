#include "process.h"
#include "uci.h"
#include "move.h"
#include "board.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void process_data(const char *pos_url, const char *eval_url, const char *out_url)
{
    srand(time(NULL));

    // open files
    FILE *positions, *evaluations, *output;
    if ((positions = fopen(pos_url, "r")) == NULL) return;
    if ((evaluations = fopen(eval_url, "r")) == NULL) return;
    if ((output = fopen(out_url, "w")) == NULL) return;

    char pos_buf[128], eval_buf[128];

    U64 position_count = 0;
    double total_err = 0;

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

            // only write quiet positions to output
            int capture_count = 0;
            move_list moves;
            generate_moves(&moves);
            for (int i = 0; i < moves.count; ++i) {
                copy_board();
                if (make_move(moves.moves[i], captures_only)) {
                    ++capture_count;
                    take_back();
                    break;
                }
                take_back();
            }

            if (!capture_count && !in_check()) {
                write_to_output(output, (side == white) ? eval : -eval);
                int crab_eval = evaluate();
                total_err += loss(crab_eval, eval);
                ++position_count;
            }
        }
    }

    fclose(positions);
    fclose(evaluations);
    fclose(output);

    printf("%llu positions written to %s\n", position_count, out_url);
    printf("MSE of crabstar eval: %0.3f\n", sqrt(total_err/position_count));
}

// writes current position and evaluation to output file
void write_to_output(FILE *fp, int evaluation)
{
    char *encoding = encode_position();
    fputs(encoding, fp);
    free(encoding);
    int crab_eval = evaluate();
    fprintf(fp, " %d %d\n", evaluation, crab_eval);
}

// calculates loss given two evaluation inputs
double loss(int eval1, int eval2)
{
    double sigmoid1 = 1 / (1 + exp(eval1*0.01));
    double sigmoid2 = 1 / (1 + exp(eval2*0.01));
    return (sigmoid1 - sigmoid2) * (sigmoid1 - sigmoid2);
}