#ifndef UCI_H
#define UCI_H

// parse UCI move string input
int parse_move(const char *move_str);
/*
    source square | target square | promoted piece (if applicable)
    examples: e2e4, e8g8, a7a8q
*/

// parse UCI position command
void parse_position(const char *command);
/*
    example commands
    - position startpos                                     // start position
    - position startpos moves e2e4 e7e5                     // init start position and make moves
    - position fen 8/8/8/8/8/8/8/8 w - - 0 1                // init position from FEN
    - position fen 8/8/8/8/8/8/8/kq5K w - - 0 1 moves h1h2  // init from FEN and make moves
*/

// parse UCI "go" command
void parse_go(const char *command);
/*
    start calculating on current position
    - depth: search x plies only -- implemented
    - nodes: search x nodes only
    - wtime: white time left (ms)
    - btime: black time left (ms)
    - winc: white increment per move (ms)
    - binc: black increment per move (ms)
*/

#endif