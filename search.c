#include <stdio.h>
#include <string.h>

#include "search.h"
#include "board.h"
#include "move.h"
#include "constants.h"
#include "timecontrol.h"
#include "hash.h"
#include "net.h"

// declaring global variables
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

// negamax alpha beta search
// alpha: minimum score the maximizing player is assured of
// beta: maximum score the minimizing player is assured of
// goal is to get alpha as high as possible
// if beta <= alpha, then the minimizing player had a better option, so we can prune
int negamax(const int alpha, const int beta, int depth, const float net_weight)
{
    int score;
    // define hash flag
    int hash_flag = hash_flag_alpha;

    // figure out if the current node is PV node or not
    int pv_node = (beta - alpha > 1);

    //read hash entry if we're not in a root ply and hash entry is available and current node is not a PV node
    if (ply && (score = read_hash_entry(alpha, beta, depth)) != no_hash_entry && !pv_node){
        // if the move has already been searched return the score for this move without searching it
        return score;
    }
    // every 2047 communicate with gui/user input
    if (!(neg_nodes & 2047))
        communicate();
    // initialize pv length
    pv_length[ply] = ply;

    if (depth == 0)         // base case
        return quiescence(alpha, beta, net_weight);
    
    ++neg_nodes;

    if (ply >= max_ply)     // too deep, danger of exceeding arrays
        return (int)(net_weight*net_eval() + (1-net_weight)*evaluate());

    // if in check, increase search depth
    int in_check = square_attacked((side==white) ? ls1b(bitboards[K]) : ls1b(bitboards[k]), side^1);
    if (in_check)
        ++depth;
    
    int legal_count = 0;
    int best_sofar = 0;
    int new_alpha = alpha;
    

    // null move pruning
    if (depth >= 3 && in_check == 0 && ply) {
        // copy_board
        copy_board();
        // increment ply
        ++ply;
        // hash enpassant if available
        if (enpassant != no_sq) hash_key ^= enpassant_keys[enpassant];
        // reset enpassant capture square
        enpassant = no_sq;
        //switch the side and give an opponent an extra move to make
        side ^= 1;
        // hash side
        hash_key ^= side_key;
        // search moves with reduced depth to find beta cutoffs
        int score = -negamax(-beta, -beta + 1, depth - 2, net_weight);
        //decrement ply
        --ply;
        //restore board state
        take_back();
        // if time is up return 0
        if (stopped) return 0;
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

        if (!moves_searched) score = -negamax(-beta, -new_alpha, depth-1, net_weight); // normal alpha beta search

        else { // late move reduction (LMR)
            //condition to consider LMR
            if (moves_searched >= full_depth_moves && depth >= reduction_limit && !in_check 
            && !get_move_capture(moves.moves[i]) && !get_move_promoted(moves.moves[i]))
                //search current move with reduced depth
                score = -negamax(-new_alpha - 1, -new_alpha, depth - 2, net_weight);
            else // hack to ensure that full-depth search is done
                score = new_alpha + 1;
            // principle variation search PVS
            if (score > new_alpha) {
                //search the remaining moves to prove they are all bad
                score = -negamax(-new_alpha -1, -new_alpha, depth-1, net_weight);
                // if a better move is found in the search, search again with normal alpha beta score bounds
                if ((score > new_alpha) && (score < beta))
                    score = -negamax(-beta, -new_alpha, depth-1, net_weight);
            }
        }

        take_back();
        --ply;
        if (stopped) return 0; // if time is up
        ++moves_searched;

        // variation is better than current best
        if (score > new_alpha) {
            // switch hash flag to one for storing PV node score
            hash_flag = hash_flag_exact;

            // if move is quiet, add move to history heuristic
            // nodes higher in the tree are valued more
            if (!get_move_capture(move))
                history_moves[get_move_piece(move)][get_move_target(move)] += 1 << depth;

            new_alpha = score;
            if (ply == 0) best_sofar = move;
            
            // update pv table and length
            pv_table[ply][ply] = move;
            for (int next_ply = ply+1; next_ply < pv_length[ply+1]; ++next_ply)     // copy next row
                pv_table[ply][next_ply] = pv_table[ply+1][next_ply];
            pv_length[ply] = pv_length[ply+1];
                    // move failed hard beta cutoff
            if (score >= beta) {
                // store hash entry with score equal to beta
                write_hash_entry(beta, depth, hash_flag_beta);
                // if move is quiet, store in killer moves cache so it has higher priority in analysis
                if (!get_move_capture(move)) {
                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = move;
                }
                return beta;
            }

        }
    }

    // if no legal moves are possible, position is either checkmate or stalemate
    if (legal_count == 0) return in_check ? -mate_value + ply : 0;

    // store hash entry with the score equal to alpha
    write_hash_entry(new_alpha, depth, hash_flag);

    if (new_alpha > alpha)      // improvement was found
        best_move = best_sofar;
    
    return new_alpha;
}

// quiescence search, see negamax code for better documentation
int quiescence(const int alpha, const int beta, const float net_weight)
{
    // every 2047 nodes
    if (!(neg_nodes & 2047))
        // listen to gui/user input
        communicate();

    ++neg_nodes;
    // int evaluation = evaluate();
    int evaluation = (int)(net_weight*net_eval() + (1-net_weight)*evaluate());

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
        int score = -quiescence(-beta, -new_alpha, net_weight);     // now from opposite perspective

        --ply;
        take_back();
        if (stopped) return 0; //time is up
        if (score >= beta) return beta;
        if (score > new_alpha) new_alpha = score;
    }

    return new_alpha;
}

// search for best move and print it
void search_position(const int max_depth, const float net_weight)
{
    printf("searching at depth %d\n", max_depth);
    // reset variables
    neg_nodes = 0;
    follow_pv = 0;
    score_pv = 0;
    stopped = 0;
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    //define intial alpha and beta bounds
    int alpha = -infinity;
    int beta = infinity;
    // iteratively deepen analysis
    int score = 0;
    for (int depth = 1; depth <= max_depth; ++depth) 
    {
        if (stopped) break; //time is up
        follow_pv = 1;
        // find best move within a given position
        score = negamax(alpha, beta, depth, net_weight);
        // we fell outide the window, so try again with a full-wdth window
        if ((score <= alpha) || (score >= beta)) {
            alpha = -infinity;
            beta = infinity;
            continue;
        }
        alpha = score - 50;
        beta = score + 50;

        if (score > -mate_value && score < -mate_score)
            printf("info score mate %d depth %d nodes %d time %d pv ", -(score + mate_value) / 2 - 1, depth, neg_nodes, get_time_ms() - starttime);
        
        else if (score > mate_score && score < mate_value)
            printf("info score mate %d depth %d nodes %d time %d pv ", (mate_value - score) / 2 + 1, depth, neg_nodes, get_time_ms() - starttime);   
        
        else
            printf("info score cp %d depth %d nodes %d time %d pv ", score, depth, neg_nodes, get_time_ms() - starttime);

        for (int i = 0 ; i < pv_length[0]; ++i) {
            printf(" ");
            print_move(pv_table[0][i]);
        }
        printf("\n");
}
        // best move
        printf("bestmove ");
        print_move(pv_table[0][0]);
        printf("\n");
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