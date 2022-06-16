#include <string.h>
#include <iostream>
#include "magicbitboard.hpp"

/*
unsigned int get_random_U32_number()
{
    // get current state
    unsigned int number = state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random number state
    state = number;

    // return random number
    return number;
}

U64 get_random_U64_number()
{
    // define 4 random numbers
    U64  n1, n2, n3, n4;

    // init random numbers slicing 16 bits from MS1B side
    n1 = (U64)(get_random_U32_number() & 0xFFFF);
    n2 = (U64)(get_random_U32_number() & 0xFFFF);
    n3 = (U64)(get_random_U32_number() & 0xFFFF);
    n4 = (U64)(get_random_U32_number() & 0xFFFF);

    //return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generate_magic_number()
{
  return (get_random_U64_number() & get_random_U64_number() & get_random_U64_number());
}

U64 find_magic_number(int square, int relevant_bits, U64 bishop)
{
    // initialize occupancies, attack mask, and used attacks
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];

    // initialize attack mask for a current piece
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // initialize occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indicies
    for (int index = 0; index < occupancy_indicies; index++)
    {
        // initialize occupancies
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);

        // initialize attacks
        attacks[index] = bishop ? generate_bishop_attacks(square, occupancies[index]) :
                                generate_rook_attacks(square, occupancies[index]);
    }

    // test magic numberse loop
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // generate magic number candidate
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        // initialize used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // test magic index loop
        for (int index = 0, fail = 0; !fail && index < occupancy_indicies; index++)
        {
            // initialize magic index
            int magic_index = (int) ((occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[index] == 0ULL)
                // initialize used attacks
                used_attacks[magic_index] = attacks[index];
            
            else if (used_attacks[index] != attacks[index])
                // magic index doesn't work
                fail = 1;

        if (!fail)
            //
            return magic_number;
        
        }
    }
    std::cout << " Magic number fails!\n";
    return 0ULL;
}

void init_magic_numbers()
{

    // loop over 64 board squares
    for (int square = 0; square < 64; square ++)
        // initialize rook magic numbers
        printf(" 0x%llxULL\n", find_magic_number(square, rook_relevant_bits[square], rook));

    printf("\n\n");

     // loop over 64 board squares
    for (int square = 0; square < 64; square ++)
        // initialize bishop magic numbers
        printf(" 0x%llxULL\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
}
*/