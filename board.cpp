#include <iostream>
#include <string.h>
#include "board.hpp"
#include "move.hpp"
#include "bitboard.hpp"
#include "leapers.hpp"
#include "sliders.hpp"

// declaring global variables
U64 bitboards[12];
U64 occupancies[3];
int side = white;
int enpassant = no_sq;
int castle = 0;

// print current state of board
void print_board()
{
    std::cout << std::endl;
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            if (!file) std::cout << "  " << 8-rank << " ";      // print ranks

            int square = rank*8 + file;
            int piece = -1;

            // loop over piece bitboards to see if any piece is present
            for (int i = 0; i < 12; ++i)
                if (get_bit(bitboards[i], square))
                    piece = i;
            
            std::cout << " ";
            #ifdef WIN64
                std::cout << ((piece == -1) ? '.' : ascii_pieces[piece]);
            #else
                std::cout << ((piece == -1) ? "." : unicode_pieces[piece]);
            #endif
        }
        std::cout << std::endl;
    }
    std::cout << "\n     a b c d e f g h\n\n";

    // print out game state variables
    std::cout << "     Side:     " << (side ? "black" : "white") << std::endl;
    std::cout << "     Enpass:      " << ((enpassant != no_sq) ? square_to_coordinates[enpassant] : "no")
              << std::endl;
    std::cout << "     Castling:  "
              << ((castle & wk) ? 'K' : '-')
              << ((castle & wq) ? 'Q' : '-')
              << ((castle & bk) ? 'k' : '-')
              << ((castle & bq) ? 'q' : '-')
              << std::endl << std::endl;
}

// initialize attack tables for leapers
void init_leapers_attacks()
{
    // loop over 64 board squres
    for (int square = 0; square < 64; ++square)
    {
        // inititialize pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);

        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);

        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

// initialize all attacks
void init_all()
{
    init_leapers_attacks();
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
                int piece = char_pieces.at(*fen);
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
int square_attacked(int square, int side)
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

// generate all moves
void generate_moves(move_list *moves, int side)
{
    
}

// generate pawn moves
void generate_pawn_moves(move_list *moves, int side)
{
    if (side == white) {
        U64 bitboard = bitboards[P];
        while (bitboard) {
            int source = get_ls1b_index(bitboard);

            // quiet moves
            int target = source - 8;
            if (!get_bit(occupancies[both], target)) {      // target square is not occupied
                if (source <= h7) {      // promotions
                    add_move(moves, encode_move(source, target, P, Q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, R, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, B, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, N, 0, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, P, 0, 0, 0, 0, 0));     // single push
                    if (source >= a2 && !get_bit(occupancies[both], source-16))         // double push
                        add_move(moves, encode_move(source, source-16, P, 0, 0, 1, 0, 0));
                }
            }

            // regular captures
            U64 attacks = pawn_attacks[white][source] & occupancies[black];
            while (attacks) {
                target = get_ls1b_index(attacks);
                if (source <= h7) {     // promotions
                    add_move(moves, encode_move(source, target, P, Q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, R, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, B, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, P, N, 1, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, P, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target);
            }

            // en passant captures
            if (enpassant != no_sq) {
                if (U64 enpas_attacks = pawn_attacks[white][source] & (1ULL << enpassant)) {
                    target = get_ls1b_index(enpas_attacks);
                    add_move(moves, encode_move(source, target, P, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, source);
        }
    } else {
        U64 bitboard = bitboards[p];
        while (bitboard) {
            int source = get_ls1b_index(bitboard);

            // quiet moves
            int target = source + 8;
            if (!get_bit(occupancies[both], target)) {
                if (source >= a2) {
                    add_move(moves, encode_move(source, target, p, q, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, r, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, b, 0, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, n, 0, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, p, 0, 0, 0, 0, 0));
                    if (source <= h7 && !get_bit(occupancies[both], source+16))
                        add_move(moves, encode_move(source, source+16, p, 0, 0, 1, 0, 0));
                }
            }

            // regular captures
            U64 attacks = pawn_attacks[black][source] & occupancies[white];
            while (attacks) {
                target = get_ls1b_index(attacks);
                if (source >= a2) {     // promotions
                    add_move(moves, encode_move(source, target, p, q, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, r, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, b, 1, 0, 0, 0));
                    add_move(moves, encode_move(source, target, p, n, 1, 0, 0, 0));
                } else {
                    add_move(moves, encode_move(source, target, p, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, target);
            }

            // en passant captures
            if (enpassant != no_sq) {
                if (U64 enpas_attacks = pawn_attacks[black][source] & (1ULL << enpassant)) {
                    target = get_ls1b_index(enpas_attacks);
                    add_move(moves, encode_move(source, target, p, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, source);
        }
    }
}


// void generate_moves()
// {
//     int source_square, target_square;
//     U64 bitboard, attacks;

//     // loop over all the bitboards
//     for (int piece = P; piece <= k; piece++)
//     {
//         bitboard = bitboards[piece];

//         // generate white pawns and white king castling moves
//         if (side == white)
//         {
//             if (piece == P) {
//                 U64 moves = (bitboard << 8) & !occupancies[both];
//             } else if (piece == p) {
//                 U64 moves = (bitboard >> 8) & !occupancies[both];
//             }
//         }

//         // generate black pawns and black king castling moves
//         else
//         {

//         }
//         // generate knight moves
        
//         // generate bishop moves

//         // generate rook moves

//         // generate queen moves

//         // generate king moves
//     }
// }