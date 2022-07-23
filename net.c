#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "bitboard.h"
#include "board.h"
#include "constants.h"

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

// initialize weight vectors
void init_weights()
{
    load_weights(weights1, nodes0*nodes1, "model/layer1-weights.txt");
    load_weights(weights2, nodes1*nodes2, "model/layer2-weights.txt");
    load_weights(weights3, nodes2*nodes3, "model/layer3-weights.txt");
    load_weights(weights4, nodes3*nodes4, "model/layer4-weights.txt");
    load_weights(biases1, nodes1, "model/layer1-biases.txt");
    load_weights(biases2, nodes2, "model/layer2-biases.txt");
    load_weights(biases3, nodes3, "model/layer3-biases.txt");
    load_weights(biases4, nodes4, "model/layer4-biases.txt");
}

// evaluate position using neural network
int net_eval()
{
    // pass through first layer
    float *prods1 = malloc(sizeof(biases1));
    memcpy(prods1, biases1, sizeof(biases1));

    // loop through bitboards
    for (int piece = 0; piece < 12; ++piece) {
        U64 bitboard = bitboards[(piece + ((side==black)*6)) % 12];
        while (bitboard) {
            int loc = ls1b(bitboard);
            int idx = piece*64 + ((side==white) ? loc : 63-loc);
            for (int i = 0; i < nodes1; ++i)
                prods1[i] += weights1[idx*nodes1 + i];
            pop_bit(bitboard, loc);
        }
    }

    // pass through second layer
    float *prods2 = malloc(sizeof(biases2));
    memcpy(prods2, biases2, sizeof(biases2));

    for (int i = 0; i < nodes1; ++i) {
        if (prods1[i] <= 0) continue;
        for (int j = 0; j < nodes2; ++j)
            prods2[j] += prods1[i] * weights2[i*nodes2 + j];
    }
    free(prods1);

    // pass through third layer
    float *prods3 = malloc(sizeof(biases3));
    memcpy(prods3, biases3, sizeof(biases3));

    for (int i = 0; i < nodes2; ++i) {
        if (prods2[i] <= 0) continue;
        for (int j = 0; j < nodes2; ++j)
            prods3[j] += prods2[i] * weights3[i*nodes2 + j];
    }
    free(prods2);

    // pass through final layer
    float final = biases4[0];
    for (int i = 0; i < nodes3; ++i) {
        if (prods3[i] <= 0) continue;
        final += prods3[i] * weights4[i];
    }
    free(prods3);
    
    return (int)(final * 100);
}