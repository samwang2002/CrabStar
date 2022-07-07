#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

// loops through moves and evaluations in given files
// creates output file containing training positions and evaluations for neural network
void process_data(const char *pos_url, const char *eval_url, const char *out_url);

// writes current position and evaluation to output file
void write_to_output(FILE *fp, int evaluation);

#endif