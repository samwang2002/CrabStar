#ifndef PERFT_H
#define PERFT_H

// get current time in milliseconds
int get_time_ms();

// recursive function that takes in depth and last move and updates associated global variables
void perft_driver(const int depth, const int last_move);

// perft test that calls perft_driver function and prints results
void perft_test(const int depth);

#endif