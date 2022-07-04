#include <stdio.h>
#include "hash.h"
#include "constants.h"

// initialize random hash keys
void init_random_keys()
{
    random_state = 1804289383;
    // loop over piece codes
    for (int piece = P; piece <= k; ++piece){
        // loop over board squres
        for (int square = 0; square < 64; ++square) {
            // initialize random piece keys
            piece_keys[piece][square] = get_random_U64_number();
        }
    }
    // generate random enpassant keys
    for (int square = 0; square < 64; ++square)
        //init random enpassant keys
        enpassant_keys[square] = get_random_U64_number();
    // generate castling keys
    for (int index = 0; index < 16; index++)
        // init castling keys
        castle_keys[index] = get_random_U64_number();
    // initialize random side key
    side_key = get_random_U64_number();


}