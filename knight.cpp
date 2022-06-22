#include "knight.hpp"
#include "constants.hpp"

U64 knight_attacks[64];

// generate knight attacks
U64 mask_knight_attacks(int square)
{
    // result attacks bitboard
    U64 attacks = 0ULL;

    // piece bitboard
    U64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    //generate knight attacks 17, 15, 10 ,6
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    // return attack map
    return attacks;
}

// initialize knight attack table
void init_knight_attacks()
{
    for (int square = 0; square <= 64; ++square)
        knight_attacks[square] = mask_knight_attacks(square);
}

// generate knight moves
void generate_knight_moves(move_list *moves, int side, U64 *bitboards, U64 *occupancies)
{
    if (side == white)
    {
        // define piece as white knight
        int piece = N;
    
        // define white knight bitboard
        U64 bitboard = bitboards[N];

        // init source square
        int source_square = get_ls1b_index(bitboard);

        // init white knight's attacks to get set of target squares
        U64 attacks = knight_attacks[source_square] & (~occupancies[white]);

        // loop over target squares available from generated attacks
        while (attacks)
        {
            // init target square
            int target_square = get_ls1b_index(attacks);

            //quiet move
            if(!get_bit(occupancies[black], target_square))
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            
            // capture move
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            
            // pop ls1b in current attacks set
            pop_bit(attacks,target_square);
        }

        //pop ls1b of the current piece bitboard copy
        pop_bit(bitboard, source_square);

    }
    
    else
    {
        // define piece as black knight
        int piece = n;
    
        // define black knight bitboard
        U64 bitboard = bitboards[n];

        // init source square
        int source_square = get_ls1b_index(bitboard);

        // init black knight's attacks to get set of target squares
        U64 attacks = knight_attacks[source_square] & (~occupancies[black]);

        // loop over target squares available from generated attacks
        while (attacks)
        {
            // init target square
            int target_square = get_ls1b_index(attacks);

            //quiet move
            if(!get_bit(occupancies[white], target_square))
                add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
            
            // capture move
            else
                add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
            
            // pop ls1b in current attacks set
            pop_bit(attacks,target_square);
        }

        //pop ls1b of the current piece bitboard copy
        pop_bit(bitboard, source_square);
    }
}