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
int killer_moves[2][64];
int history_moves[12][64];
int pv_length[64];
int pv_table[64][64];
int follow_pv;
int score_pv;
int full_depth_moves = 4;
int reduction_limit = 3;

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

// get priority of move
int score_move(const int move)
{
    if (score_pv){              // if PV move scoring is allowed
        if (pv_table[0][ply] == move){   // make sure we are dealing with PV move]
            score_pv = 0; //disable score PV flag
            return 20000;  //give PV move the highest score to search it first
        } 
    }
    if (get_move_capture(move)) {           // capture move, use mvv_lva table
        int target_piece = 0;
        for (int i = p; i <= k; ++i)
            if (get_bit(bitboards[i-6*side], get_move_target(move))) {
                target_piece = i-6*side;
                break;
            }
        return mvv_lva[get_move_piece(move)][target_piece] + 10000;
    } else {                                // score quiet move
        if (killer_moves[0][ply] == move) return 9000;
        if (killer_moves[1][ply] == move) return 8000;
        return history_moves[get_move_piece(move)][get_move_target(move)];
    }
    return 0;
}

// sort moves by priority
void sort_moves(move_list *moves)
{
    // make list of current scores
    int scores[moves->count];
    for (int i = 0; i < moves->count; ++i)
        scores[i] = score_move(moves->moves[i]);

    // insertion sort in descending order (fast for small arrays)
    for (int i = 0; i < moves->count-1; ++i) {
        // find index of largest element in unsorted part
        int max_idx = i;
        for (int j = i+1; j < moves->count; ++j)
            if (scores[j] > scores[max_idx]) max_idx = j;
        
        // swap scores
        int temp_score = scores[max_idx];
        scores[max_idx] = scores[i];
        scores[i] = temp_score;

        // swap moves
        int temp_move = moves->moves[max_idx];
        moves->moves[max_idx] = moves->moves[i];
        moves->moves[i] = temp_move;
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

// negamax alpha beta search
// alpha: minimum score the maximizing player is assured of
// beta: maximum score the minimizing player is assured of
// goal is to get alpha as high as possible
// if beta <= alpha, then the minimizing player had a better option, so we can prune
int negamax(const int alpha, const int beta, int depth)
{

    // initialize pv length
    pv_length[ply] = ply;

    if (depth == 0)         // base case
        return quiescence(alpha, beta);
    
    ++neg_nodes;

    if (ply >= max_ply)     // too deep, danger of exceeding arrays
        return evaluate();

    // if in check, increase search depth
    int in_check = square_attacked((side==white) ? ls1b(bitboards[K]) : ls1b(bitboards[k]), side^1);
    if (in_check)
        ++depth;
    
    int legal_count = 0;
    int best_sofar = 0;
    int new_alpha = alpha;
    int score;

    // null move pruning
    if (depth >= 3 && in_check == 0 && ply) {
        // copy_board
        copy_board();
        //switch the side and give an opponent an extra move to make
        side ^= 1;
        // reset enpassant capture square
        enpassant = no_sq;
        // search moves with reduced depth to find beta cutoffs
        int score = -negamax(-beta, -beta + 1, depth -1 -2);
        //restore board state
        take_back();
        // fail-hard beta cutoff
        if (score >= beta)
            //move fails high
            return beta;
    }

    // loop through possible moves and narrow alpha and beta
    move_list moves;
    generate_moves(&moves);

    // if we are following PV line
    if (follow_pv)
        enable_pv_scoring(&moves);
    sort_moves(&moves);

    // moves searched in a move list
    int moves_searched = 0;

    for (int i = 0; i < moves.count; ++i) {
        copy_board();
        ++ply;

        int move = moves.moves[i];
        if (!make_move(move, all_moves)) {      // skip illegal moves
            --ply;
            continue;
        }
        ++legal_count;

        if (!moves_searched) score = -negamax(-beta, -new_alpha, depth - 1); // normal alpha beta search

        else { // late move reduction (LMR)
            //condition to consider LMR
            if (moves_searched >= full_depth_moves && depth >= reduction_limit && !in_check 
            && !get_move_capture(moves.moves[i]) && !get_move_promoted(moves.moves[i]))
                //search current move with reduced depth
                score = -negamax(-new_alpha - 1, -new_alpha, depth -2);
            else // hack to ensure that full-depth search is done
                score = new_alpha + 1;
            // principle variation search PVS
            if (score > new_alpha) {
                score = -negamax(-new_alpha -1, -new_alpha, depth-1); //search the remaining moves to prove they are all bad
                // if a better move is found in the search, search again with normal alpha beta score bounds
                if ((score > new_alpha) && (score < beta))
                    score = -negamax(-beta, -new_alpha, depth-1);
            }
        }

        take_back();
        --ply;
        ++moves_searched;

        // move failed hard beta cutoff
        if (score >= beta) {
            // if move is quiet, store in killer moves cache so it has higher priority in analysis
            if (!get_move_capture(move)) {
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = move;
            }
            return beta;
        }

        // variation is better than current best
        if (score > new_alpha) {
            new_alpha = score;
            if (ply == 0) best_sofar = move;
            
            // update pv table and length
            pv_table[ply][ply] = move;
            for (int next_ply = ply+1; next_ply < pv_length[ply+1]; ++next_ply)     // copy next row
                pv_table[ply][next_ply] = pv_table[ply+1][next_ply];
            pv_length[ply] = pv_length[ply+1];

            // if move is quiet, add move to history heuristic
            // nodes higher in the tree are valued more
            if (!get_move_capture(move))
                history_moves[get_move_piece(move)][get_move_target(move)] += 1 << depth;
        }
    }

    // if no legal moves are possible, position is either checkmate or stalemate
    if (legal_count == 0) return in_check ? -49000 + ply : 0;

    if (new_alpha > alpha)      // improvement was found
        best_move = best_sofar;
    
    return new_alpha;
}

// quiescence search, see negamax code for better documentation
int quiescence(const int alpha, const int beta)
{
    ++neg_nodes;
    int evaluation = evaluate();

    if (evaluation >= beta) return beta;                // failed hard beta cutoff
    int new_alpha = alpha;
    if (evaluation > alpha) new_alpha = evaluation;     // found better move

    // DFS over possible captures
    move_list moves;
    generate_moves(&moves);
    sort_moves(&moves);

    for (int i = 0; i < moves.count; ++i) {
        copy_board();
        ++ply;

        // test out new move
        int move = moves.moves[i];
        if (make_move(move, captures_only) == 0) {      // skip illegal and quiet moves
            --ply;
            continue;
        }
        int score = -quiescence(-beta, -new_alpha);     // now from opposite perspective

        --ply;
        take_back();
        if (score >= beta) return beta;
        if (score > new_alpha) new_alpha = score;
    }

    return new_alpha;
}

// search for best move and print it
void search_position(const int max_depth)
{
    // reset variables
    neg_nodes = 0;
    follow_pv = 0;
    score_pv = 0;
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    //define intial alpha and beta bounds
    int alpha = -50000;
    int beta = 50000;
    // iteratively deepen analysis
    int score = 0;
    for (int depth = 1; depth <= max_depth; ++depth) 
    {
        follow_pv = 1;
        // find best move within a given position
        score = negamax(alpha, beta, depth);
        // we fell outide the window, so try again with a full-wdth window
        if ((score <= alpha) || (score >= beta)) {
            alpha = -50000;
            beta = 50000;
            continue;
        }
        alpha = score - 50;
        beta = score + 50;
        //if (best_move) {
            // basic info
            printf("info score cp %d depth %d nodes %d", score, depth, neg_nodes);

            // principal variation
            printf(" pv");
            for (int i = 0 ; i < pv_length[0]; ++i) {
                printf(" ");
                print_move(pv_table[0][i]);
            }
            printf("\n");
    }
            // best move
            printf("bestmove ");
            print_move(best_move);
            printf("\n");
        //} else
        //    printf("no best move found\n");
    //}
}

// enable PV move scoring
void enable_pv_scoring(move_list *moves)
{
    //disable following PV
    follow_pv =0;
    // loop over the moves within a move list
    for (int i = 0; i < moves->count; ++i){
        // make sure we hit PV move
        if(pv_table[0][ply] == moves->moves[i]){
            //enable move scoring
            score_pv = 1;

            //enable following PV
            follow_pv = 1;
        }
    }
}