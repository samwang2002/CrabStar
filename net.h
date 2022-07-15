#ifndef NET_H
#define NET_H

/*
    neural net structure:
    768 input nodes: 12 piece bitboards, all from perspective of player to move
    dense 128 node layer with ReLU activation
    dense 32 node layer with ReLU activation
    dense 32 node layer with ReLU activation
    1 output
*/

#define nodes0 768
#define nodes1 128
#define nodes2 32
#define nodes3 32
#define nodes4 1

// struct to contain all the weights in a network
typedef struct {
    float weights1[nodes0 * nodes1];
    float weights2[nodes1 * nodes2];
    float weights3[nodes2 * nodes3];
    float weights4[nodes3 * nodes4];

    float biases1[nodes1];
    float biases2[nodes2];
    float biases3[nodes3];
    float biases4[nodes4];
} net_weights;

// load weights from text file into 1d array
void load_weights(float *weights, const int dim, const char *path);

// initialize weight vectors in weights given folder path
void read_weights(net_weights *weights, const char *path);

// evaluate position using neural network
int net_eval(const net_weights *weights);

// ------------ handling net evolution ------------
// copy weights from one network to the other
void copy_weights(net_weights *destination, net_weights *source);

// add normal(0, std_dev^2) noise randomly to one in inv_rate weight parameters
void mutate(net_weights *weights, const int inv_rate, const float std_dev);

#endif