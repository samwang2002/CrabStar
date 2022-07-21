#ifndef CONSTANTS_H
#define CONSTANTS_H

// -------------------- BOARD STATE  --------------------
// sides to move (colors)
enum {white, black, both};

// encode pieces
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

// convert ASCII character pieces to encoded constants
static int char_pieces[] = {
    ['P'] = P, ['N'] = N, ['B'] = B, ['R'] = R, ['Q'] = Q, ['K'] = K,
    ['p'] = p, ['n'] = n, ['b'] = b, ['r'] = r, ['q'] = q, ['k'] = k
};

// castling rights binary encoding
enum { wk = 1, wq = 2, bk = 4, bq = 8 };
/*
    bin  dec
    
   0001    1  white king can castle to the king side
   0010    2  white king can castle to the queen side
   0100    4  black king can castle to the king side
   1000    8  black king can castle to the queen side
   examples
   1111       both sides an castle both directions
   1001       black king => queen side
              white king => king side
*/

// castling rights update constants
static const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};
/*
                           castling   move     in      in
                              right update     binary  decimal
 king & rooks didn't move:     1111 & 1111  =  1111    15
        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7
*/


// -------------------- EVALUATION  --------------------
//material scores
static const int material_score[12] = {
    100,        // white pawn score
    300,        // white knight score
    350,        // white bishop score
    500,        // white rook score
    1000,       // white queen score
    10000,      // white king score
    -100,       // black pawn score
    -300,       // black knight score
    -350,       // black bishop score
    -500,       // black rook score
    -1000,      // black queen score
    -10000      // black king score
};

// positional scores
// pawn positional score
static const int pawn_score[64] = 
{
    90,  90,  90,  90,  90,  90,  90,  90,
    30,  30,  30,  40,  40,  30,  30,  30,
    20,  20,  20,  30,  30,  30,  20,  20,
    10,  10,  10,  20,  20,  10,  10,  10,
     5,   5,  10,  20,  20,   5,   5,   5,
     0,   0,   0,   5,   5,   0,   0,   0,
     0,   0,   0, -10, -10,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
static const int knight_score[64] = 
{
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,  10,  10,   0,   0,  -5,
    -5,   5,  20,  20,  20,  20,   5,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,  10,  20,  30,  30,  20,  10,  -5,
    -5,   5,  20,  10,  10,  20,   5,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
static const int bishop_score[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,  10,  10,   0,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,  10,   0,   0,   0,   0,  10,   0,
     0,  30,   0,   0,   0,   0,  30,   0,
     0,   0, -10,   0,   0, -10,   0,   0

};

// rook positional score
static const int rook_score[64] =
{
    50,  50,  50,  50,  50,  50,  50,  50,
    50,  50,  50,  50,  50,  50,  50,  50,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,  10,  20,  20,  10,   0,   0,
     0,   0,   0,  20,  20,   0,   0,   0

};

// king positional score
static const int king_score[64] = 
{
     0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   5,   5,   5,   5,   0,   0,
     0,   5,   5,  10,  10,   5,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   5,  10,  20,  20,  10,   5,   0,
     0,   0,   5,  10,  10,   5,   0,   0,
     0,   5,   5,  -5,  -5,   0,   5,   0,
     0,   0,   5,   0, -15,   0,  10,   0
};

// file masks [square]
static const U64 file_masks[64] = {
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL,
    0x101010101010101ULL,
    0x202020202020202ULL,
    0x404040404040404ULL,
    0x808080808080808ULL,
    0x1010101010101010ULL,
    0x2020202020202020ULL,
    0x4040404040404040ULL,
    0x8080808080808080ULL
};

// rank masks [square]
static const U64 rank_masks[64] = {
    0xffULL,
    0xffULL,
    0xffULL,
    0xffULL,
    0xffULL,
    0xffULL,
    0xffULL,
    0xffULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff00ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff0000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff00000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff0000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL,
    0xff00000000000000ULL
};

// isolated pawn masks [square]
// all bits in adjacent files are set
static const U64 isolated_masks[64] = {
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL,
    0x202020202020202ULL,
    0x505050505050505ULL,
    0xa0a0a0a0a0a0a0aULL,
    0x1414141414141414ULL,
    0x2828282828282828ULL,
    0x5050505050505050ULL,
    0xa0a0a0a0a0a0a0a0ULL,
    0x4040404040404040ULL
};

// passed pawn masks [color][square]
static const U64 passed_masks[2][64] = {
    {
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x3ULL,
        0x7ULL,
        0xeULL,
        0x1cULL,
        0x38ULL,
        0x70ULL,
        0xe0ULL,
        0xc0ULL,
        0x303ULL,
        0x707ULL,
        0xe0eULL,
        0x1c1cULL,
        0x3838ULL,
        0x7070ULL,
        0xe0e0ULL,
        0xc0c0ULL,
        0x30303ULL,
        0x70707ULL,
        0xe0e0eULL,
        0x1c1c1cULL,
        0x383838ULL,
        0x707070ULL,
        0xe0e0e0ULL,
        0xc0c0c0ULL,
        0x3030303ULL,
        0x7070707ULL,
        0xe0e0e0eULL,
        0x1c1c1c1cULL,
        0x38383838ULL,
        0x70707070ULL,
        0xe0e0e0e0ULL,
        0xc0c0c0c0ULL,
        0x303030303ULL,
        0x707070707ULL,
        0xe0e0e0e0eULL,
        0x1c1c1c1c1cULL,
        0x3838383838ULL,
        0x7070707070ULL,
        0xe0e0e0e0e0ULL,
        0xc0c0c0c0c0ULL,
        0x30303030303ULL,
        0x70707070707ULL,
        0xe0e0e0e0e0eULL,
        0x1c1c1c1c1c1cULL,
        0x383838383838ULL,
        0x707070707070ULL,
        0xe0e0e0e0e0e0ULL,
        0xc0c0c0c0c0c0ULL,
        0x3030303030303ULL,
        0x7070707070707ULL,
        0xe0e0e0e0e0e0eULL,
        0x1c1c1c1c1c1c1cULL,
        0x38383838383838ULL,
        0x70707070707070ULL,
        0xe0e0e0e0e0e0e0ULL,
        0xc0c0c0c0c0c0c0ULL
    }, {
        0x303030303030300ULL,
        0x707070707070700ULL,
        0xe0e0e0e0e0e0e00ULL,
        0x1c1c1c1c1c1c1c00ULL,
        0x3838383838383800ULL,
        0x7070707070707000ULL,
        0xe0e0e0e0e0e0e000ULL,
        0xc0c0c0c0c0c0c000ULL,
        0x303030303030000ULL,
        0x707070707070000ULL,
        0xe0e0e0e0e0e0000ULL,
        0x1c1c1c1c1c1c0000ULL,
        0x3838383838380000ULL,
        0x7070707070700000ULL,
        0xe0e0e0e0e0e00000ULL,
        0xc0c0c0c0c0c00000ULL,
        0x303030303000000ULL,
        0x707070707000000ULL,
        0xe0e0e0e0e000000ULL,
        0x1c1c1c1c1c000000ULL,
        0x3838383838000000ULL,
        0x7070707070000000ULL,
        0xe0e0e0e0e0000000ULL,
        0xc0c0c0c0c0000000ULL,
        0x303030300000000ULL,
        0x707070700000000ULL,
        0xe0e0e0e00000000ULL,
        0x1c1c1c1c00000000ULL,
        0x3838383800000000ULL,
        0x7070707000000000ULL,
        0xe0e0e0e000000000ULL,
        0xc0c0c0c000000000ULL,
        0x303030000000000ULL,
        0x707070000000000ULL,
        0xe0e0e0000000000ULL,
        0x1c1c1c0000000000ULL,
        0x3838380000000000ULL,
        0x7070700000000000ULL,
        0xe0e0e00000000000ULL,
        0xc0c0c00000000000ULL,
        0x303000000000000ULL,
        0x707000000000000ULL,
        0xe0e000000000000ULL,
        0x1c1c000000000000ULL,
        0x3838000000000000ULL,
        0x7070000000000000ULL,
        0xe0e0000000000000ULL,
        0xc0c0000000000000ULL,
        0x300000000000000ULL,
        0x700000000000000ULL,
        0xe00000000000000ULL,
        0x1c00000000000000ULL,
        0x3800000000000000ULL,
        0x7000000000000000ULL,
        0xe000000000000000ULL,
        0xc000000000000000ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL,
        0x0ULL
    }
};

// extracts rank from a square [square]
static const int get_rank[64] = {
    7, 7, 7, 7, 7, 7, 7, 7,
    6, 6, 6, 6, 6, 6, 6, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
};

// -- positional advantages and disadvantages --
// penalty for each doubled pawn
static const int doubled_pen = -10;
// penalty for each isolated pawn
static const int isolated_pen = -10;
// bonus for passed pawns on different ranks
static const int passed_bonus[8] = {0, 5, 10, 20, 35, 60, 100, 200};

// bonus for rook on semi-open file, penalty for king
static const int semi_bonus = 10;
// bonus for rook on open file, penalty for king
static const int open_bonus = 15;
// bonus for each square bishops and queens can access (personal guess)
static const int mobility_bonus = 2;
// bonus for each place within 1 square of king
static const int shield_bonus = 5;

// mirror positional score tables for opposite side
static const int mirror_score[128] =
{
	a1, b1, c1, d1, e1, f1, g1, h1,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a8, b8, c8, d8, e8, f8, g8, h8
};


// -------------------- SEARCH PRUNING  --------------------
// most valuable victim and least valuable attacker table [attacker][victim]
static const int mvv_lva[12][12] = {
    {105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
    {104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
    {103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
    {102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
    {101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
    {100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600},

    {105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605},
    {104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604},
    {103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603},
    {102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602},
    {101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601},
    {100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600},
};
/*
    higher score means that a capture is considered first
    order of pawn, knight, bishop, rook, queen, king
    example: pawn captures queen (very important) => 505,
             queen captures pawn (not as important) => 101
*/

// move scores
// [-infinity, -mate_value ... -mate_score, ... score ... mate_score ... mate_value, infinity]
#define infinity 50000
#define mate_value 49000
#define mate_score 48000

#define horizontal_line "------------------------"

#endif