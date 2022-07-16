#include "random.h"
#include <stdlib.h>
#include <math.h>

float gaussian_rand()
{
    float u = (float)rand() / (float)RAND_MAX;
    float v = (float)rand() / (float)RAND_MAX;
    return sqrt(-2*log(u)) * cos(2*M_PI*v);
}