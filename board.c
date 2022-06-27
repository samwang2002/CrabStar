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
U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;
int ply = 0;
int best_move = 0;
int neg_nodes = 0;

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
}

// initialize all attacks
void init_attacks()
{
    init_pawn_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_bishop_attacks();
    init_rook_attacks();
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

// generate all moves
void generate_moves(move_list *moves)
{
    moves->count = 0;
    generate_pawn_moves(moves, side, bitboards, occupancies, enpassant);
    generate_knight_moves(moves, side, bitboards, occupancies);
    generate_castling_moves(moves, side, bitboards, occupancies, castle, &square_attacked);
    generate_king_moves(moves, side, bitboards, occupancies);
    generate_bishop_moves(moves, side, bitboards, occupancies);
    generate_rook_moves(moves, side, bitboards, occupancies);
    generate_queen_moves(moves, side, bitboards, occupancies);
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

        // clear target square
        if (capture) {
            if (side == white) {
                for (int i = p; i <= k; ++i)
                    pop_bit(bitboards[i], target);
                pop_bit(occupancies[black], target);
            } else {
                for (int i = P; i <= K; ++i)
                    pop_bit(bitboards[i], target);
                pop_bit(occupancies[white], target);
            }
            // don't need to clear occupancy for both colors on target square
        }

        // place piece on target square
        if (promoted_piece)
            set_bit(bitboards[promoted_piece], target);
        else
            set_bit(bitboards[piece], target);
        set_bit(occupancies[side], target);
        set_bit(occupancies[both], target);

        // handle enpassant captures
        if (enpass) {
            if (side == white) {
                pop_bit(bitboards[p], target+8);
                pop_bit(occupancies[black], target+8);
                pop_bit(occupancies[both], target+8);
            } else {
                pop_bit(bitboards[P], target-8);
                pop_bit(occupancies[white], target-8);
                pop_bit(occupancies[both], target-8);
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
        if (double_push)
            enpassant = target + ((side==white) ? 8 : -8);
        
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
                    break;
                case c1:
                    pop_bit(bitboards[R], a1);
                    pop_bit(occupancies[white], a1);
                    pop_bit(occupancies[both], a1);
                    set_bit(bitboards[R], d1);
                    set_bit(occupancies[white], d1);
                    set_bit(occupancies[both], d1);
                    break;
                case g8:
                    pop_bit(bitboards[r], h8);
                    pop_bit(occupancies[black], h8);
                    pop_bit(occupancies[both], h8);
                    set_bit(bitboards[r], f8);
                    set_bit(occupancies[black], f8);
                    set_bit(occupancies[both], f8);
                    break;
                case c8:
                    pop_bit(bitboards[r], a8);
                    pop_bit(occupancies[black], a8);
                    pop_bit(occupancies[both], a8);
                    set_bit(bitboards[r], d8);
                    set_bit(occupancies[black], d8);
                    set_bit(occupancies[both], d8);
                    break;
            }
        }

        // update castling rights
        castle &= castling_rights[source] & castling_rights[target];

        // flip side
        side ^= 1;

        return 1;
    } else          // quiet move, ignore because flag asks only for captures
        return 0;
}

// search for best move and print it
void search_position(const int depth)
{
    // find best move within a given position
    int score = negamax(-50000, 50000, depth);
    
    if (best_move)
    {
        printf("info score cp %d depth %d nodes %d\n", score, depth, neg_nodes);
        // PLACEHOLDER: returns first move in move list
        printf("bestmove ");
        print_move(best_move);
        printf("\n");
    }
}

// evaulate the current board position
int evaluate()
{
    //static evaluation score
    int score = 0;

    // current pieces bitboard copy
    U64 bitboard;

    // initialize piece and square
    int piece, square;

    // loop over piece bitboards
    for (int bb_piece = P; bb_piece<= k; bb_piece++)
    {
        // init piece bitboard copy
        bitboard = bitboards[bb_piece];


        // loop over pieces within a bitboard
        while (bitboard)
        {
            // init piece
            piece = bb_piece;  

            // init square
            square = ls1b(bitboard);
            
            // score material weights
            score += material_score[piece];

            // score positional piece scores
            switch (piece)
            {
                //evaluate white pieces
                case P: score += pawn_score[square]; break;
                case N: score += knight_score[square]; break;
                case B: score += bishop_score[square]; break;
                case R: score += rook_score[square]; break;
                //case Q: score += (rook_score[square] + bishop_score[square]); break;
                case K: score += king_score[square]; break;

                //evaulate black pieces
                case p: score -= pawn_score[mirror_score[square]]; break;
                case n: score -= knight_score[mirror_score[square]]; break;
                case b: score -= bishop_score[mirror_score[square]]; break;
                case r: score -= rook_score[mirror_score[square]]; break;
                //case q: score -= (rook_score[mirror_score[square]] + bishop_score[mirror_score[square]]); break;
                case k: score -= king_score[mirror_score[square]]; break;
            }

            // pop ls1b
            pop_bit(bitboard, square);
        }
    }

    // return final evaluation based on side
    return (side == white) ? score : -score;
}

int negamax(int alpha, int beta, int depth)
{
    // recursion escape condition
    if (depth == 0)
        return evaluate();
    
    // increment nodes count
    neg_nodes++;
    
    // is king in check
    int in_check = square_attacked((side == white) ? ls1b(bitboards[K]) : ls1b(bitboards[k]), side^1);

    // legal moves counter
    int legal_moves = 0;

    // best move so far
    int best_sofar;
    
    // old value of alpha
    int old_alpha = alpha;

    //create move list instance
    move_list moves[1];

    //generate moves
    generate_moves(moves);

    // loop over moves within a movelist
    for (int count = 0; count < moves->count; count++)
    {
        //preserve board state
        copy_board();
        
        //increment ply
        ply++;

        // make sure to only make legal moves
        if (!make_move(moves->moves[count], all_moves))
        {
            // decrement ply
            ply --;

            // skip to next move
            continue;
        }

        // increment legal moves
        legal_moves++;
        
        // score current move
        int score = -negamax(-beta, -alpha, depth -1);
        
        // decrement ply
        ply --;

        // take move back
        take_back();

        // fail-hard beta cutoff
        if (score >= beta)
        {
            // node (move) fails high
            return beta;
        }

        // found a better move
        if (score > alpha)
        {
            // principle variation node (move)
            alpha = score;

            // if root move
            if (ply == 0)
                // associate bets move with the best score
                best_sofar = moves->moves[count];
        }
    }

    // no legal moves to make in the current position
    if (legal_moves == 0)
    {
        // checkmate
        if(in_check)
            // return mating score
            return -49000 + ply;
        
        // stalemate
        else
            //
            return 0;
    }

    if (old_alpha != alpha)
        best_move = best_sofar;

    // node (move) fails low
    return alpha;

}