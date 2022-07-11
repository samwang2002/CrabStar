#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "move.h"
#include "bitboard.h"
#include "constants.h"
#include "timecontrol.h"
#include "hash.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

// declaring global variables
U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;
int ply = 0;

// print current state of board
void print_board()
{
    printf("\n");
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (!file) printf("  %d ", 8-rank);             // print ranks

            int square = rank*8 + file;
            int piece = -1;

            // loop over piece bitboards to see if any piece is present
            for (int i = 0; i < 12; ++i)
                if (get_bit(bitboards[i], square))
                    piece = i;
            
            #ifdef WIN64
                printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
            #else
                printf(" %s", (piece == -1) ? "." : unicode_pieces[piece]);
            #endif
        }
        printf("\n");
    }
    printf("\n     a b c d e f g h\n\n");

    // print out game state variables
    printf("     Side:     %s\n", side ? "black" : "white");
    printf("     Enpass:      %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-',
           (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-');
    // print hash key
    printf("     Hash key:  %llx\n", hash_key);
}

// initialize all attacks
void init_all()
{
    init_pawn_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_bishop_attacks();
    init_rook_attacks();
    init_random_keys();
    clear_hash_table();
}

// parse fen from string
void parse_fen(const char *fen)
{
    // reset board position and state variables
    memset(bitboards, 0, sizeof(bitboards));
    memset(occupancies, 0, sizeof(occupancies));
    side = 0;
    enpassant = no_sq;
    castle = 0;

    // load position
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank*8 + file;
            if (isalpha(*fen)) {            // piece
                int piece = char_pieces[(int)*fen];
                set_bit(bitboards[piece], square);
            } else if (isdigit(*fen)) {     // empty squares
                int offset = *fen - '0';
                file += offset-1;       // subtract one because file automatically increments
            }
            ++fen;
        }
        ++fen;
    }

    // get game state variables
    // side
    (*fen == 'w') ? (side=white) : (side=black);
    fen += 2;

    // castling
    while (*fen != ' ') {
        switch (*fen) {
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
        }
        ++fen;
    }
    ++fen;

    // en passant square
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank*8 + file;
    } else enpassant = no_sq;

    // initialize occupancies
    for (int piece = P; piece <= K; ++piece) occupancies[white] |= bitboards[piece];
    for (int piece = p; piece <= k; ++piece) occupancies[black] |= bitboards[piece];
    occupancies[both] |= occupancies[white] | occupancies[black];

    // initialize hash key
    hash_key = generate_hash_key();
}

// get whether square is currently attacked by given side
int square_attacked(const int square, const int side)
{
    if (side == white) {
        U64 attacks = (pawn_attacks[black][square] & bitboards[P])              // not a typo
                    | (knight_attacks[square] & bitboards[N])
                    | (get_bishop_attacks(square, occupancies[both]) & bitboards[B])
                    | (get_rook_attacks(square, occupancies[both]) & bitboards[R])
                    | (get_queen_attacks(square, occupancies[both]) & bitboards[Q])
                    | (king_attacks[square] & bitboards[K]);
        return attacks ? 1 : 0;
    } else {
        U64 attacks = (pawn_attacks[white][square] & bitboards[p])
                    | (knight_attacks[square] & bitboards[n])
                    | (get_bishop_attacks(square, occupancies[both]) & bitboards[b])
                    | (get_rook_attacks(square, occupancies[both]) & bitboards[r])
                    | (get_queen_attacks(square, occupancies[both]) & bitboards[q])
                    | (king_attacks[square] & bitboards[k]);
        return attacks ? 1 : 0;
    }
}

// get whether side has legal moves
int has_legal_moves()
{
    copy_board();
    move_list moves;
    generate_moves(&moves);
    for (int i = 0; i < moves.count; ++i) {
        if (make_move(moves.moves[i], all_moves)) {
            take_back();
            return 1;
        }
    }
    return 0;
}

// generate all moves least important moves first
void generate_moves(move_list *moves)
{
    moves->count = 0;
    generate_pawn_moves(moves, side, bitboards, occupancies, enpassant);
    generate_castling_moves(moves, side, bitboards, occupancies, castle, &square_attacked);
    generate_knight_moves(moves, side, bitboards, occupancies);
    generate_bishop_moves(moves, side, bitboards, occupancies);
    generate_rook_moves(moves, side, bitboards, occupancies);
    generate_queen_moves(moves, side, bitboards, occupancies);
    generate_king_moves(moves, side, bitboards, occupancies);
}

// make move on chess board
// move_flag is either all_moves or captures_only
// if move_flag == captures_only, make_move will only update the board if the given move is a capture
int make_move(const int move, const int move_flag)
{
    if (move_flag == all_moves || get_move_capture(move)) {

        // preserve board state
        copy_board();

        // parse move
        int source = get_move_source(move);
        int target = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double(move);
        int enpass = get_move_enpassant(move);
        int castling = get_move_castling(move);

        // -- update board position --
        // clear source square
        pop_bit(bitboards[piece], source);
        pop_bit(occupancies[side], source);
        pop_bit(occupancies[both], source);
        hash_key ^= piece_keys[piece][source];

        // clear target square
        if (capture) {
            if (side == white) {
                for (int i = p; i <= k; ++i){
                    if (get_bit(bitboards[i], target)){
                        pop_bit(bitboards[i], target);
                        hash_key ^= piece_keys[i][target];
                        break;
                    }
                }
                pop_bit(occupancies[black], target);
            } else {
                for (int i = P; i <= K; ++i){
                    if (get_bit(bitboards[i], target)) {
                        pop_bit(bitboards[i], target);
                        hash_key ^= piece_keys[i][target];
                    }
                }
                pop_bit(occupancies[white], target);
            }
            // don't need to clear occupancy for both colors on target square
        }

        // place piece on target square
        if (promoted_piece) {
            set_bit(bitboards[promoted_piece], target);
            hash_key ^= piece_keys[promoted_piece][target];
        }
        else {
            set_bit(bitboards[piece], target);
            hash_key ^= piece_keys[piece][target];
        }
        set_bit(occupancies[side], target);
        set_bit(occupancies[both], target);

        // handle enpassant captures
        if (enpass) {
            if (side == white) {
                pop_bit(bitboards[p], target+8);
                pop_bit(occupancies[black], target+8);
                pop_bit(occupancies[both], target+8);
                hash_key ^= piece_keys[p][target+8];
            } else {
                pop_bit(bitboards[P], target-8);
                pop_bit(occupancies[white], target-8);
                pop_bit(occupancies[both], target-8);
                hash_key ^= piece_keys[P][target-8];
            }
        }
        enpassant = no_sq;

        // check if move is legal
        if (square_attacked((side == white) ? ls1b(bitboards[K]) : ls1b(bitboards[k]), side^1)) {
            take_back();
            return 0;
        }

        // -- update state variables --
        // handle double pawn pushes
        if (double_push) {
            enpassant = target + ((side==white) ? 8 : -8);
            hash_key ^= enpassant_keys[target + ((side == white) ? 8 : -8)];   
        }
        // handle castling
        if (castling) {
            switch (target) {
                case g1:
                    pop_bit(bitboards[R], h1);
                    pop_bit(occupancies[white], h1);
                    pop_bit(occupancies[both], h1);
                    set_bit(bitboards[R], f1);
                    set_bit(occupancies[white], f1);
                    set_bit(occupancies[both], f1);
                    hash_key ^= piece_keys[R][h1];
                    hash_key ^= piece_keys[R][f1];
                    break;
                case c1:
                    pop_bit(bitboards[R], a1);
                    pop_bit(occupancies[white], a1);
                    pop_bit(occupancies[both], a1);
                    set_bit(bitboards[R], d1);
                    set_bit(occupancies[white], d1);
                    set_bit(occupancies[both], d1);
                    hash_key ^= piece_keys[R][a1];
                    hash_key ^= piece_keys[R][d1];
                    break;
                case g8:
                    pop_bit(bitboards[r], h8);
                    pop_bit(occupancies[black], h8);
                    pop_bit(occupancies[both], h8);
                    set_bit(bitboards[r], f8);
                    set_bit(occupancies[black], f8);
                    set_bit(occupancies[both], f8);
                    hash_key ^= piece_keys[r][h8];
                    hash_key ^= piece_keys[r][f8];
                    break;
                case c8:
                    pop_bit(bitboards[r], a8);
                    pop_bit(occupancies[black], a8);
                    pop_bit(occupancies[both], a8);
                    set_bit(bitboards[r], d8);
                    set_bit(occupancies[black], d8);
                    set_bit(occupancies[both], d8);
                    hash_key ^= piece_keys[r][a8];
                    hash_key ^= piece_keys[r][d8];
                    break;
            }
        }

        hash_key ^= castle_keys[castle];
        // update castling rights
        castle &= castling_rights[source] & castling_rights[target];
        hash_key ^= castle_keys[castle];

        // flip side
        side ^= 1;
        hash_key ^= side_key;

        return 1;
    } else          // quiet move, ignore because flag asks only for captures
        return 0;
}

// evaulate the current board position
int evaluate()
{
    int score = 0;

    // loop over piece bitboards
    for (int bb_piece = P; bb_piece<= k; bb_piece++) {
        U64 bitboard = bitboards[bb_piece];
        while (bitboard) {
            int piece = bb_piece;
            int square = ls1b(bitboard);
            int doubled_pawns = 0;
            score += material_score[piece];

            // score positional piece scores
            switch (piece) {
                case P:
                    score += pawn_score[square];
                    doubled_pawns = count_bits(bitboards[P] & file_masks[square]);
                    if (doubled_pawns > 1)                              // doubled
                        score += doubled_pawns * doubled_pen;
                    if ((bitboards[P] & isolated_masks[square]) == 0)   // isolated
                        score += isolated_pen;
                    if ((passed_masks[white][square] & bitboards[p]) == 0)  // passed
                        score += passed_bonus[get_rank[square]];
                    break;
                
                case N: score += knight_score[square]; break;

                case B:
                    score += bishop_score[square];
                    score += mobility_bonus *
                             count_bits(get_bishop_attacks(square, occupancies[both])); // mobility bonus
                    break;

                case R:
                    score += rook_score[square];
                    if ((bitboards[P] & file_masks[square]) == 0) {         // semi open file bonus
                        score += semi_bonus;
                        if ((bitboards[p] & file_masks[square]) == 0)       // open file bonus
                            score += open_bonus;
                    }
                    break;
                
                case Q:
                    score += rook_score[square] + bishop_score[square];
                    score += mobility_bonus *
                             count_bits(get_queen_attacks(square, occupancies[both]));  // mobility bonus
                    break;

                case K:
                    score += king_score[square];
                    if ((bitboards[P] & file_masks[square]) == 0) {         // semi open file penalty
                        score -= semi_bonus;
                        if ((bitboards[p] & file_masks[square]) == 0)       // open file penalty
                            score -= open_bonus;
                    }
                    score += shield_bonus * count_bits(king_attacks[square] & occupancies[white]);  // shield
                    break;


                case p:
                    score -= pawn_score[mirror_score[square]];
                    doubled_pawns = count_bits(bitboards[p] & file_masks[square]);
                    if (doubled_pawns > 1)                              // doubled
                        score -= doubled_pawns * doubled_pen;
                    if ((bitboards[p] & isolated_masks[square]) == 0)   // isolated
                        score -= isolated_pen;
                    if ((passed_masks[black][square] & bitboards[P]) == 0)  // passed
                        score -= passed_bonus[get_rank[mirror_score[square]]];
                    break;
                
                case n: score -= knight_score[mirror_score[square]]; break;

                case b:
                    score -= bishop_score[mirror_score[square]];
                    score -= mobility_bonus *
                             count_bits(get_bishop_attacks(square, occupancies[both])); // mobility bonus
                    break;

                case r:
                    score -= rook_score[mirror_score[square]];
                    if ((bitboards[p] & file_masks[square]) == 0) {         // semi open file bonus
                        score -= semi_bonus;
                        if ((bitboards[P] & file_masks[square]) == 0)       // open file bonus
                            score -= open_bonus;
                    }
                    break;
                
                case q:
                    score -= rook_score[mirror_score[square]] + bishop_score[mirror_score[square]];
                    score -= mobility_bonus *
                             count_bits(get_queen_attacks(square, occupancies[both]));  // mobility bonus
                    break;

                case k:
                    score -= king_score[mirror_score[square]];
                    if ((bitboards[p] & file_masks[square]) == 0) {         // semi open file penalty
                        score += semi_bonus;
                        if ((bitboards[P] & file_masks[square]) == 0)       // open file penalty
                            score += open_bonus;
                    }
                    score -= shield_bonus * count_bits(king_attacks[square] & occupancies[black]);  // shield
                    break;
            }
            pop_bit(bitboard, square);
        }
    }
    return (side == white) ? score : -score;
}