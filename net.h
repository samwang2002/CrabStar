#ifndef NET_H
#define NET_H

#define nodes1 768
#define nodes2 128
#define nodes3 32
#define nodes4 32

// load weights from text file into 1d array
void load_weights(float *weights, const int dim, const char *path);

#endif