#include "uci.h"
#include "move.h"
#include "board.h"
#include "constants.h"

// parse move string input
int parse_move(const char *move_str)
{
    // parse data from string
    int source = (move_str[0]-'a') + (8 - (move_str[1]-'0')) * 8;
    int target = (move_str[2]-'a') + (8 - (move_str[3]-'0')) * 8;

    // generate pseudolegal moves and look for match
    move_list moves;
    generate_moves(&moves);

    for (int i = 0; i < moves.count; ++i) {
        int move = moves.moves[i];
        if (source == get_move_source(move) && target == get_move_target(move)) {
            int prom_piece = get_move_promoted(move);
            if (prom_piece) {
                if ((prom_piece == Q || prom_piece == q) && move_str[4] == 'q') return move;
                if ((prom_piece == R || prom_piece == r) && move_str[4] == 'r') return move;
                if ((prom_piece == B || prom_piece == b) && move_str[4] == 'b') return move;
                if ((prom_piece == N || prom_piece == n) && move_str[4] == 'n') return move;
            } else
                return move;
        }
    }

    // no match, move is illegal
    return 0;
}