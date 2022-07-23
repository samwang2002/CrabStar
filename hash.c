#include <stdio.h>
#include "hash.h"
#include "constants.h"
#include "board.h"

tt hash_table[hash_size];
lockless_ht shared_ht[hash_size];

U64 piece_keys[12][64];
U64 enpassant_keys[64];
U64 castle_keys[16];
U64 side_key;
U64 hash_key;

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

void clear_hash_table()
{
    // loop over TT elements
    for (int i= 0; i < hash_size; ++i)
    {
        // reset TT inner fields
        hash_table[i].hash_key = 0;
        hash_table[i].depth = 0;
        hash_table[i].flag = 0;
        hash_table[i].score = 0;
    }
    // loop over shared HT elements
    for (int i = 0; i < hash_size; ++i)
    {
        shared_ht[i].hash_key = 0;
        shared_ht[i].depth = 0;
        shared_ht[i].data = 0;
    }
}

int read_hash_entry(int alpha, int beta, int depth)
{
    // create a TT instance pointer
    tt *hash_entry = &hash_table[hash_key % hash_size];

    //make sure we're dealing with exact position we need
    if (hash_entry->hash_key == hash_key)
    {
        // make sure that we match the exact depth our search is at
        if (hash_entry->depth >= depth)
        {
            // extract stored score from TT entry
            int score = hash_entry->score;

            // retrieve score independent from actual path
            if (score < -mate_score) score += ply;
            if (score > mate_score) score -= ply;
            
            // match the exact PV node score
            if (hash_entry->flag == hash_flag_exact)
                return score;
            // match alpha (fail-low node) score
            if (hash_entry->flag == hash_flag_alpha && score <= alpha)
                return alpha;
                // match beta (fail-high node) score
            if (hash_entry->flag == hash_flag_beta && score >= beta)
                return beta;
        }
    }
    return no_hash_entry;

}

void write_hash_entry(int score, int depth, int hash_flag)
{
    // create a TT instance pointer to particular hash entry storing
    tt *hash_entry = &hash_table[hash_key % hash_size];

    if (score < -mate_score) score -= ply;
    if (score > mate_score) score += ply;

    // write hash entry data 
    hash_entry->hash_key = hash_key;
    hash_entry->score = score;
    hash_entry->flag = hash_flag;
    hash_entry->depth = depth;
}

int read_shared_entry(int alpha, int beta, int depth)
{
    lockless_ht *hash_entry = &shared_ht[hash_key % hash_size];

    if ((hash_entry->hash_key ^ hash_entry->data) == hash_key)
    {

        int score = ((hash_entry->data & 0xFFFFFFFF00000000) >> 32);

        if (score < -mate_score) score += ply;
        if (score > mate_score) score -= ply;

        int flag = hash_entry->data & 0x00000000FFFFFFFF;
        
        if (flag == hash_flag_exact)
            return score;
        
        if (flag == hash_flag_alpha && score <= alpha)
            return alpha;

        if (flag == hash_flag_beta && score >= beta)
            return beta;
    }

    return no_hash_entry;
}

void write_shared_entry(int score, int depth, int hash_flag)
{
    lockless_ht *hash_entry = &shared_ht[hash_key % hash_size];

    if (score < -mate_score) score -= ply;
    if (score > mate_score) score += ply;
    
    U64 data = 0;
    U64 data_score = score;
    data_score >>= 32;
    data ^= data_score;
    data ^= hash_flag;

    hash_entry->data = data;
    hash_entry->depth = depth;
    hash_entry->hash_key = (hash_key ^ data);

}