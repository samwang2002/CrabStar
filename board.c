#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "move.h"
#include "bitboard.h"
#include "constants.h"

#include "pawn.h"
#include "knight.h"
#include "king.h"
#include "bishop.h"
#include "rook.h"
#include "queen.h"

// declaring global variables
// U64 bitboards[12];
// U64 occupancies[3];
// int side = white;
// int enpassant = no_sq;
// int castle = 0;
// int ply = 0;

// generate castling moves
void generate_castling_moves(board_state *board, move_list *moves, const int side, const U64 *bitboards,
                            const U64 *occupancies, const int castle)
{
    if (side == white) {
        if ((castle & wk) && !get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1)
            && !square_attacked(board, e1, black) && !square_attacked(board, f1, black)
            && !square_attacked(board, g1, black))
                add_move(moves, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
        if ((castle & wq) && !get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1)
            && !get_bit(occupancies[both], b1) && !square_attacked(board, e1, black)
            && !square_attacked(board, d1, black) && !square_attacked(board, c1, black))
                add_move(moves, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
    } else {
        if ((castle & bk) && !get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8)
            && !square_attacked(board, e8, white) && !square_attacked(board, f8, white)
            && !square_attacked(board, g8, white))
            add_move(moves, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
        if ((castle & bq) && !get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8)
            && !get_bit(occupancies[both], b8) && !square_attacked(board, e8, white)
            && !square_attacked(board, d8, white) && !square_attacked(board, c8, white))
                add_move(moves, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
    }
}

// print current state of board
void print_board(const board_state *board)
{
    printf("\n");
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (!file) printf("  %d ", 8-rank);             // print ranks

            int square = rank*8 + file;
            int piece = -1;

            // loop over piece bitboards to see if any piece is present
            for (int i = 0; i < 12; ++i)
                if (get_bit(board->bitboards[i], square))
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
    printf("     Side:     %s\n", board->side ? "black" : "white");
    printf("     Enpass:      %s\n", (board->enpassant != no_sq) ? square_to_coordinates[board->enpassant] : "no");
    printf("     Castling:  %c%c%c%c\n\n", (board->castle & wk) ? 'K' : '-', (board->castle & wq) ? 'Q' : '-',
           (board->castle & bk) ? 'k' : '-', (board->castle & bq) ? 'q' : '-');
}

// initialize all attacks
void init_all()
{
    init_pawn_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_bishop_attacks();
    init_rook_attacks();
}

// parse fen from string
void parse_fen(board_state *board, const char *fen)
{
    // reset board position and state variables
    memset(board->bitboards, 0, sizeof(board->bitboards));
    memset(board->occupancies, 0, sizeof(board->occupancies));
    board->side = 0;
    board->enpassant = no_sq;
    board->castle = 0;

    // load position
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank*8 + file;
            if (isalpha(*fen)) {            // piece
                int piece = char_pieces[(int)*fen];
                set_bit(board->bitboards[piece], square);
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
    (*fen == 'w') ? (board->side=white) : (board->side=black);
    fen += 2;

    // castling
    while (*fen != ' ') {
        switch (*fen) {
            case 'K': board->castle |= wk; break;
            case 'Q': board->castle |= wq; break;
            case 'k': board->castle |= bk; break;
            case 'q': board->castle |= bq; break;
        }
        ++fen;
    }
    ++fen;

    // en passant square
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        board->enpassant = rank*8 + file;
    } else board->enpassant = no_sq;

    // initialize occupancies
    for (int piece = P; piece <= K; ++piece) board->occupancies[white] |= board->bitboards[piece];
    for (int piece = p; piece <= k; ++piece) board->occupancies[black] |= board->bitboards[piece];
    board->occupancies[both] |= board->occupancies[white] | board->occupancies[black];
}

// get whether square is currently attacked by given side
int square_attacked(board_state *board, const int square, const int side)
{
    if (side == white) {
        U64 attacks = (pawn_attacks[black][square] & board->bitboards[P])              // not a typo
                    | (knight_attacks[square] & board->bitboards[N])
                    | (get_bishop_attacks(square, board->occupancies[both]) & board->bitboards[B])
                    | (get_rook_attacks(square, board->occupancies[both]) & board->bitboards[R])
                    | (get_queen_attacks(square, board->occupancies[both]) & board->bitboards[Q])
                    | (king_attacks[square] & board->bitboards[K]);
        return attacks ? 1 : 0;
    } else {
        U64 attacks = (pawn_attacks[white][square] & board->bitboards[p])
                    | (knight_attacks[square] & board->bitboards[n])
                    | (get_bishop_attacks(square, board->occupancies[both]) & board->bitboards[b])
                    | (get_rook_attacks(square, board->occupancies[both]) & board->bitboards[r])
                    | (get_queen_attacks(square, board->occupancies[both]) & board->bitboards[q])
                    | (king_attacks[square] & board->bitboards[k]);
        return attacks ? 1 : 0;
    }
}

// get whether side has legal moves
int has_legal_moves(board_state *board)
{
    board_state board_copy;
    memcpy(&board_copy, board, sizeof(board_state));

    move_list moves;
    generate_moves(board, &moves);
    for (int i = 0; i < moves.count; ++i) {
        if (make_move(board, moves.moves[i], all_moves)) {
            memcpy(board, &board_copy, sizeof(board_state));
            return 1;
        }
    }
    return 0;
}

// generate all moves least important moves first
void generate_moves(board_state *board, move_list *moves)
{
    moves->count = 0;
    generate_pawn_moves(moves, board->side, board->bitboards, board->occupancies, board->enpassant);
    generate_castling_moves(board, moves, board->side, board->bitboards, board->occupancies, board->castle);
    generate_knight_moves(moves, board->side, board->bitboards, board->occupancies);
    generate_bishop_moves(moves, board->side, board->bitboards, board->occupancies);
    generate_rook_moves(moves, board->side, board->bitboards, board->occupancies);
    generate_queen_moves(moves, board->side, board->bitboards, board->occupancies);
    generate_king_moves(moves, board->side, board->bitboards, board->occupancies);
}

// make move on chess board
// move_flag is either all_moves or captures_only
// if move_flag == captures_only, make_move will only update the board if the given move is a capture
int make_move(board_state *board, const int move, const int move_flag)
{
    if (move_flag == all_moves || get_move_capture(move)) {
        // preserve board state
        board_state board_copy;
        memcpy(&board_copy, board, sizeof(board_state));

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
        pop_bit(board->bitboards[piece], source);
        pop_bit(board->occupancies[board->side], source);
        pop_bit(board->occupancies[both], source);

        // clear target square
        if (capture) {
            if (board->side == white) {
                for (int i = p; i <= k; ++i){
                    if (get_bit(board->bitboards[i], target)){
                        pop_bit(board->bitboards[i], target);
                        break;
                    }
                }
                pop_bit(board->occupancies[black], target);
            } else {
                for (int i = P; i <= K; ++i){
                    if (get_bit(board->bitboards[i], target)) {
                        pop_bit(board->bitboards[i], target);
                    }
                }
                pop_bit(board->occupancies[white], target);
            }
            // don't need to clear occupancy for both colors on target square
        }

        // place piece on target square
        if (promoted_piece) set_bit(board->bitboards[promoted_piece], target);
        else set_bit(board->bitboards[piece], target);
        set_bit(board->occupancies[board->side], target);
        set_bit(board->occupancies[both], target);

        // handle enpassant captures
        if (enpass) {
            if (board->side == white) {
                pop_bit(board->bitboards[p], target+8);
                pop_bit(board->occupancies[black], target+8);
                pop_bit(board->occupancies[both], target+8);
            } else {
                pop_bit(board->bitboards[P], target-8);
                pop_bit(board->occupancies[white], target-8);
                pop_bit(board->occupancies[both], target-8);
            }
        }
        board->enpassant = no_sq;

        // check if move is legal
        if (square_attacked(board, (board->side == white) ? ls1b(board->bitboards[K]) : ls1b(board->bitboards[k]),
            (board->side)^1)) {
            memcpy(board, &board_copy, sizeof(board_state));
            return 0;
        }

        // -- update state variables --
        // handle double pawn pushes
        if (double_push)
            board->enpassant = target + ((board->side==white) ? 8 : -8);

        // handle castling
        if (castling) {
            switch (target) {
                case g1:
                    pop_bit(board->bitboards[R], h1);
                    pop_bit(board->occupancies[white], h1);
                    pop_bit(board->occupancies[both], h1);
                    set_bit(board->bitboards[R], f1);
                    set_bit(board->occupancies[white], f1);
                    set_bit(board->occupancies[both], f1);
                    break;
                case c1:
                    pop_bit(board->bitboards[R], a1);
                    pop_bit(board->occupancies[white], a1);
                    pop_bit(board->occupancies[both], a1);
                    set_bit(board->bitboards[R], d1);
                    set_bit(board->occupancies[white], d1);
                    set_bit(board->occupancies[both], d1);
                    break;
                case g8:
                    pop_bit(board->bitboards[r], h8);
                    pop_bit(board->occupancies[black], h8);
                    pop_bit(board->occupancies[both], h8);
                    set_bit(board->bitboards[r], f8);
                    set_bit(board->occupancies[black], f8);
                    set_bit(board->occupancies[both], f8);
                    break;
                case c8:
                    pop_bit(board->bitboards[r], a8);
                    pop_bit(board->occupancies[black], a8);
                    pop_bit(board->occupancies[both], a8);
                    set_bit(board->bitboards[r], d8);
                    set_bit(board->occupancies[black], d8);
                    set_bit(board->occupancies[both], d8);
                    break;
            }
        }

        // update castling rights
        board->castle &= castling_rights[source] & castling_rights[target];

        // flip side
        board->side ^= 1;

        return 1;
    } else          // quiet move, ignore because flag asks only for captures
        return 0;
}

// evaulate the current board position
int evaluate(const board_state *board)
{
    int score = 0;

    // loop over piece bitboards
    for (int bb_piece = P; bb_piece<= k; bb_piece++) {
        U64 bitboard = board->bitboards[bb_piece];
        while (bitboard) {
            int piece = bb_piece;
            int square = ls1b(bitboard);
            int doubled_pawns = 0;
            score += material_score[piece];

            // score positional piece scores
            switch (piece) {
                case P:
                    score += pawn_score[square];
                    doubled_pawns = count_bits(board->bitboards[P] & file_masks[square]);
                    if (doubled_pawns > 1)                              // doubled
                        score += doubled_pawns * doubled_pen;
                    if ((board->bitboards[P] & isolated_masks[square]) == 0)   // isolated
                        score += isolated_pen;
                    if ((passed_masks[white][square] & board->bitboards[p]) == 0)  // passed
                        score += passed_bonus[get_rank[square]];
                    break;
                
                case N: score += knight_score[square]; break;

                case B:
                    score += bishop_score[square];
                    score += mobility_bonus *
                             count_bits(get_bishop_attacks(square, board->occupancies[both])); // mobility bonus
                    break;

                case R:
                    score += rook_score[square];
                    if ((board->bitboards[P] & file_masks[square]) == 0) {         // semi open file bonus
                        score += semi_bonus;
                        if ((board->bitboards[p] & file_masks[square]) == 0)       // open file bonus
                            score += open_bonus;
                    }
                    break;
                
                case Q:
                    score += rook_score[square] + bishop_score[square];
                    score += mobility_bonus *
                             count_bits(get_queen_attacks(square, board->occupancies[both]));  // mobility bonus
                    break;

                case K:
                    score += king_score[square];
                    if ((board->bitboards[P] & file_masks[square]) == 0) {         // semi open file penalty
                        score -= semi_bonus;
                        if ((board->bitboards[p] & file_masks[square]) == 0)       // open file penalty
                            score -= open_bonus;
                    }
                    score += shield_bonus * count_bits(king_attacks[square] & board->occupancies[white]);  // shield
                    break;


                case p:
                    score -= pawn_score[mirror_score[square]];
                    doubled_pawns = count_bits(board->bitboards[p] & file_masks[square]);
                    if (doubled_pawns > 1)                              // doubled
                        score -= doubled_pawns * doubled_pen;
                    if ((board->bitboards[p] & isolated_masks[square]) == 0)   // isolated
                        score -= isolated_pen;
                    if ((passed_masks[black][square] & board->bitboards[P]) == 0)  // passed
                        score -= passed_bonus[get_rank[mirror_score[square]]];
                    break;
                
                case n: score -= knight_score[mirror_score[square]]; break;

                case b:
                    score -= bishop_score[mirror_score[square]];
                    score -= mobility_bonus *
                             count_bits(get_bishop_attacks(square, board->occupancies[both])); // mobility bonus
                    break;

                case r:
                    score -= rook_score[mirror_score[square]];
                    if ((board->bitboards[p] & file_masks[square]) == 0) {         // semi open file bonus
                        score -= semi_bonus;
                        if ((board->bitboards[P] & file_masks[square]) == 0)       // open file bonus
                            score -= open_bonus;
                    }
                    break;
                
                case q:
                    score -= rook_score[mirror_score[square]] + bishop_score[mirror_score[square]];
                    score -= mobility_bonus *
                             count_bits(get_queen_attacks(square, board->occupancies[both]));  // mobility bonus
                    break;

                case k:
                    score -= king_score[mirror_score[square]];
                    if ((board->bitboards[p] & file_masks[square]) == 0) {         // semi open file penalty
                        score += semi_bonus;
                        if ((board->bitboards[P] & file_masks[square]) == 0)       // open file penalty
                            score += open_bonus;
                    }
                    score -= shield_bonus * count_bits(king_attacks[square] & board->occupancies[black]);  // shield
                    break;
            }
            pop_bit(bitboard, square);
        }
    }
    return (board->side == white) ? score : -score;
}