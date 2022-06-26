#include "bishop.h"
#include "constants.h"

// returns squares that could be obstacles in bishop's path, so edge squares are not considered
U64 mask_bishop_attacks(const int square)
{
    U64 attacks = 0ULL;

    // target rank and files
    int tr = square / 8;
    int tf = square % 8;

    // mask relevant bishop occupancy bits
    for (int r = tr+1, f=tf+1; r <= 6 && f <= 6; ++r, ++f) attacks |= (1ULL << (r*8 + f));
    for (int r = tr-1, f=tf+1; r >= 1 && f <= 6; --r, ++f) attacks |= (1ULL << (r*8 + f));
    for (int r = tr+1, f=tf-1; r <= 6 && f >= 1; ++r, --f) attacks |= (1ULL << (r*8 + f));
    for (int r = tr-1, f=tf-1; r >= 1 && f >= 1; --r, --f) attacks |= (1ULL << (r*8 + f));

    return attacks;
}

// generate bishop attacks given square and occupancies, used to fill attack table
U64 generate_bishop_attacks(const int square, const U64 block)
{
     U64 attacks = 0ULL;

    // target rank and files
     int tr = square / 8;
     int tf = square % 8;

     // mask relevant bishop occupancy bits
     for (int r = tr+1, f=tf+1; r <= 7 && f <= 7; ++r, ++f)     // bottom right
     {
        attacks |= (1ULL << (r*8 + f));
        if((1ULL << (r*8 + f) & block)) break;
     }
     for (int r = tr-1, f=tf+1; r >= 0 && f <= 7; --r, ++f)     // top right
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f) & block)) break;
     }
     for (int r = tr+1, f=tf-1; r <= 7 && f >= 0; ++r, --f)     // bottom left
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f)) & block) break;
     }
     for (int r = tr-1, f=tf-1; r >= 0 && f >= 0; --r, --f)     // top left
     {
         attacks |= (1ULL << (r*8 + f));
         if((1ULL << (r*8 + f)) & block) break;
     }
     
     return attacks;
}

// fill table of bishop attacks
void init_bishop_attacks()
{
    for (int square = 0; square < 64; ++square) {
        U64 attack_mask = mask_bishop_attacks(square);
        int bits_count = count_bits(attack_mask);
        int occupancy_idxs = (1 << bits_count);
        bishop_masks[square] = attack_mask;

        for (int index = 0; index < occupancy_idxs; ++index) {
            U64 occupancy = set_occupancy(index, bits_count, attack_mask);
            int magic_idx = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
            bishop_attacks[square][magic_idx] = generate_bishop_attacks(square, occupancy);
        }
    }
}

// get bishop attacks using magic bitboard
U64 get_bishop_attacks(const int square, const U64 occupancy)
{
    return bishop_attacks[square][((occupancy & bishop_masks[square]) * bishop_magic_numbers[square])
                                  >> (64 - bishop_relevant_bits[square])];
}

// generate bishop moves
void generate_bishop_moves(move_list *moves, const int side, const U64 *bitboards, const U64 *occupancies)
{
    if (side == white)
    {
    // define piece as white bishop
        int piece = B;
    
        // define white bishop bitboard
        U64 bitboard = bitboards[B];

        // loop over source squares of piece bitboard
        while(bitboard)
        {
            // init source square
            int source_square = ls1b(bitboard);

            // init white bishop's attacks to get set of target squares
            U64 attacks = get_bishop_attacks(source_square, occupancies[both]) & (~occupancies[white]);

            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = ls1b(attacks);

                //quiet move
                if (!get_bit(occupancies[black], target_square))
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                
                // capture move
                else
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                
                //pop ls1b in current attacks set
                pop_bit(attacks, target_square);

            }

            //pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }
    }

    else
    {
        // define piece as black bishop
        int piece = b;
    
        // define black bishop bitboard
        U64 bitboard = bitboards[b];

        // loop over source squares of piece bitboard
        while(bitboard)
        {
            // init source square
            int source_square = ls1b(bitboard);
            // init black bishop's attacks to get set of target squares
            U64 attacks = get_bishop_attacks(source_square, occupancies[both]) & ~occupancies[black];
            // loop over target squares available from generated attacks
            while (attacks)
            {
                // init target square
                int target_square = ls1b(attacks);
                //quiet move
                if (!get_bit(occupancies[white], target_square))
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                // capture move
                else
                    add_move(moves, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));

                //pop ls1b in current attacks set
                pop_bit(attacks, target_square);

            }
            //pop ls1b of the current piece bitboard copy
            pop_bit(bitboard, source_square);
        }

    }
}
