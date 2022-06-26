#ifndef UCI_H
#define UCI_H

// parse UCI move string input
int parse_move(const char *move_str);

// parse UCI position command
void parse_position(const char *command);
/*
    example commands
    - position startpos                                     // start position
    - position startpos moves e2e4 e7e5                     // init start position and make moves
    - position fen 8/8/8/8/8/8/8/8 w - - 0 1                // init position from FEN
    - position fen 8/8/8/8/8/8/8/kq5K w - - 0 1 moves h1h2  // init from FEN and make moves
*/

#endif