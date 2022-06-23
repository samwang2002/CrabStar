#ifndef CONSTANTS_H
#define CONSTANTS_H

// sides to move (colors)
enum {white, black, both};

// encode pieces
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

// convert ASCII character pieces to encoded constants
static int char_pieces[] = {
    ['P'] = P, ['N'] = N, ['B'] = B, ['R'] = R, ['Q'] = Q, ['K'] = K,
    ['p'] = p, ['n'] = n, ['b'] = b, ['r'] = r, ['q'] = q, ['k'] = k
};

//castling rights binary encoding
enum { wk = 1, wq = 2, bk = 4, bq = 8 };
/*
    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side
   examples
   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side
*/

#endif