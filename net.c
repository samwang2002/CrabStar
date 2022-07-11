#include "net.h"
#include <stdio.h>
#include <stdlib.h>

// load weights from text file into 1d array
void load_weights(float *weights, const int dim, const char *path)
{
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL) return;

    // read in lines
    char buf[128];
    for (int idx = 0; idx < dim; ++idx) {
        fgets(buf, sizeof(buf), fp);
        float w = atof(buf);
        weights[idx] = w;
    }
}