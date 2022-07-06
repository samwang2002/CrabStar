#include <stdio.h>
#include "hash.h"
#include "constants.h"
#include "board.h"

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
// generate unique position identifier from scratch
U64 generate_hash_key()
{
    U64 final_key = 0ULL; // final hash key
    U64 bitboard; // temp piece bitboard copy
    for (int piece = P; piece <= k; piece++)
    {
        bitboard = bitboards[piece]; //initialize piece bitboard copy

        // loop over pieces in the bitboard
        while (bitboard)
        {
            int square = ls1b(bitboard);
            final_key ^= piece_keys[piece][square];
            pop_bit(bitboard, square);
        }
    }
    // hash enpassant if on board
    if (enpassant != no_sq)
        final_key ^= enpassant_keys[enpassant];
    
    //hash castling rights
    final_key ^= castle_keys[castle];

    // hash side if black to move
    if (side == black) final_key ^= side_key;

    return final_key;
}