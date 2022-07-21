#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "bitboard.h"
#include "board.h"
#include "constants.h"
#include "random.h"

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
    fclose(fp);
}

// initialize weight vectors
void read_weights(net_weights *weights, const char *path)
{
    char file_path[100];

    sprintf(file_path, "%s%s", path, "/layer1-weights.txt");
    load_weights(weights->weights1, nodes0*nodes1, file_path);
    sprintf(file_path, "%s%s", path, "/layer2-weights.txt");
    load_weights(weights->weights2, nodes1*nodes2, file_path);
    sprintf(file_path, "%s%s", path, "/layer3-weights.txt");
    load_weights(weights->weights3, nodes2*nodes3, file_path);
    sprintf(file_path, "%s%s", path, "/layer4-weights.txt");
    load_weights(weights->weights4, nodes3*nodes4, file_path);
    sprintf(file_path, "%s%s", path, "/layer1-biases.txt");
    load_weights(weights->biases1, nodes1, file_path);
    sprintf(file_path, "%s%s", path, "/layer2-biases.txt");
    load_weights(weights->biases2, nodes2, file_path);
    sprintf(file_path, "%s%s", path, "/layer3-biases.txt");
    load_weights(weights->biases3, nodes3, file_path);
    sprintf(file_path, "%s%s", path, "/layer4-biases.txt");
    load_weights(weights->biases4, nodes4, file_path);
}

// evaluate position using neural network
int net_eval(const board_state *board, const net_weights *weights)
{
    // pass through first layer
    float prods1[nodes1];
    memcpy(prods1, weights->biases1, sizeof(weights->biases1));

    // loop through bitboards
    for (int piece = 0; piece < 12; ++piece) {
        U64 bitboard = board->bitboards[(piece + ((board->side==black)*6)) % 12];
        while (bitboard) {
            int loc = ls1b(bitboard);
            int idx = piece*64 + ((board->side==white) ? loc : 63-loc);
            for (int i = 0; i < nodes1; ++i)
                prods1[i] += weights->weights1[idx*nodes1 + i];
            pop_bit(bitboard, loc);
        }
    }

    // pass through second layer
    float prods2[nodes2];
    memcpy(prods2, weights->biases2, sizeof(weights->biases2));

    for (int i = 0; i < nodes1; ++i) {
        if (prods1[i] <= 0) continue;
        for (int j = 0; j < nodes2; ++j)
            prods2[j] += prods1[i] * weights->weights2[i*nodes2 + j];
    }

    // pass through third layer
    float prods3[nodes3];
    memcpy(prods3, weights->biases3, sizeof(weights->biases3));

    for (int i = 0; i < nodes2; ++i) {
        if (prods2[i] <= 0) continue;
        for (int j = 0; j < nodes2; ++j)
            prods3[j] += prods2[i] * weights->weights3[i*nodes2 + j];
    }

    // pass through final layer
    float final = weights->biases4[0];
    for (int i = 0; i < nodes3; ++i) {
        if (prods3[i] <= 0) continue;
        final += prods3[i] * weights->weights4[i];
    }
    
    return (int)(final * 100);
}

// returns copy of network
net_weights *duplicate_weights(const net_weights *weights)
{
    net_weights *copy = malloc(sizeof(net_weights));
    for (int i = 0; i < nodes0*nodes1; ++i) copy->weights1[i] = weights->weights1[i];
    for (int i = 0; i < nodes1*nodes2; ++i) copy->weights2[i] = weights->weights2[i];
    for (int i = 0; i < nodes2*nodes3; ++i) copy->weights3[i] = weights->weights3[i];
    for (int i = 0; i < nodes3*nodes4; ++i) copy->weights4[i] = weights->weights4[i];
    for (int i = 0; i < nodes1; ++i) copy->biases1[i] = weights->biases1[i];
    for (int i = 0; i < nodes2; ++i) copy->biases2[i] = weights->biases2[i];
    for (int i = 0; i < nodes3; ++i) copy->biases3[i] = weights->biases3[i];
    for (int i = 0; i < nodes4; ++i) copy->biases4[i] = weights->biases4[i];
    return copy;
}

// mutate weights in network
void mutate(net_weights *weights, const int inv_rate, const float std_dev)
{
    for (float *w = weights->weights1; w < weights->weights1 + nodes0*nodes1; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->weights2; w < weights->weights2 + nodes1*nodes2; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->weights3; w < weights->weights3 + nodes2*nodes3; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->weights4; w < weights->weights4 + nodes3*nodes4; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    
    for (float *w = weights->biases1; w < weights->biases1 + nodes1; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->biases2; w < weights->biases1 + nodes2; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->biases3; w < weights->biases1 + nodes3; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
    for (float *w = weights->biases4; w < weights->biases1 + nodes4; ++w)
        if (rand() % inv_rate == 0) *w += gaussian_rand() * std_dev;
}