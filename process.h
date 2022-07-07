#ifndef PROCESS_H
#define PROCESS_H

// loops through moves and evaluations in given files
// creates output file containing training positions and evaluations for neural network
void process_data(const char *positions_url, const char *evals_url);

#endif