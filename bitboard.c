// headers
#include <stdio.h>
#include "bitboard.h"

// get index of least significant bit
int ls1b(const U64 bitboard)
{
    if (bitboard) return count_bits((bitboard & -bitboard)-1);
    return -1;
}

// print bitboard
void print_bitboard(const U64 bitboard)
{
    // loop over board ranks
    for (int rank = 0; rank < 8; ++rank)
    {
        // loop over board files
        for (int file = 0; file < 8; ++file)
        {
            // convert file & rank into square index
            int square = rank * 8 + file;

            // print ranks
            if (!file)
                printf(" %d ", 8-rank);

            // print bit state (either 1 or 0)
            printf(" %c", get_bit(bitboard, square) ? '1' : '.');

        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n     a b c d e f g h\n\n");

    // print bitboard as unsigned decimal number
    printf("     Bitboard: %llud\n\n", bitboard);

}

// index: integer where the ith bit is set iff the ith LSB within the attack mask is occupied
// attack mask: bitboard of attack mask
// returns bitboard with the indexed squares occupied
U64 set_occupancy(const int index, const int masked_bits, U64 attack_mask)
{
    U64 occupancy = 0ULL;

    for (int count = 0; count < masked_bits; ++count) {
        int square = ls1b(attack_mask);
        attack_mask &= attack_mask-1;
        if (index & (1 << count))       // make sure occupancy is on board
            occupancy |= (1ULL << square);
    }

    return occupancy;
}

// generate 64-bit pseudo legal numbers
U64 get_random_U64_number()
{
    // define 4 random numbers
    U64 n1, n2, n3, n4;
    
    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    n4 = (U64)(get_random_U32_number()) & 0xFFFF;
    
    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// generate 32-bit pseudo legal numbers
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}