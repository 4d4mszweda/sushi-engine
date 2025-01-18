#include "evaluate.h"

char start_position[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// piece encoding
enum pieces
{
    e,
    P,
    N,
    B,
    R,
    Q,
    K,
    p,
    n,
    b,
    r,
    q,
    k,
    o
};

// square encoding
enum squares
{
    a8 = 0,
    b8,
    c8,
    d8,
    e8,
    f8,
    g8,
    h8,
    a7 = 16,
    b7,
    c7,
    d7,
    e7,
    f7,
    g7,
    h7,
    a6 = 32,
    b6,
    c6,
    d6,
    e6,
    f6,
    g6,
    h6,
    a5 = 48,
    b5,
    c5,
    d5,
    e5,
    f5,
    g5,
    h5,
    a4 = 64,
    b4,
    c4,
    d4,
    e4,
    f4,
    g4,
    h4,
    a3 = 80,
    b3,
    c3,
    d3,
    e3,
    f3,
    g3,
    h3,
    a2 = 96,
    b2,
    c2,
    d2,
    e2,
    f2,
    g2,
    h2,
    a1 = 112,
    b1,
    c1,
    d1,
    e1,
    f1,
    g1,
    h1,
    no_sq
};

// capture flags
enum capture_flags
{
    all_moves,
    only_captures
};

// castling binary representation
//
//  bin  dec
// 0001    1  white king can castle to the king side
// 0010    2  white king can castle to the queen side
// 0100    4  black king can castle to the king side
// 1000    8  black king can castle to the queen side
//
// examples
// 1111       both sides an castle both directions
// 1001       black king => queen side
//            white king => king side

// castling writes
enum castling
{
    KC = 1,
    QC = 2,
    kc = 4,
    qc = 8
};

// encode ascii pieces
int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k,
};

// material scrore

/*
    p =   100   = p
    n =   300   = p * 3
    b =   350   = p * 3 + p * 0.5
    r =   500   = p * 5
    q =   1000  = p * 10
    k =   10000 = p * 100

*/

int material_score[13] = {
    0,      // empty square score
    100,    // white pawn score
    300,    // white knight scrore
    350,    // white bishop score
    500,    // white rook score
    1000,   // white queen score
    10000,  // white king score
    -100,   // black pawn score
    -300,   // black knight scrore
    -350,   // black bishop score
    -500,   // black rook score
    -1000,  // black queen score
    -10000, // black king score

};

// castling rights

/*
                             castle   move     in      in
                              right    map     binary  decimal

        white king  moved:     1111 & 1100  =  1100    12
  white king: rook moved:     1111 & 1110  =  1110    14
 white queen: rook moved:     1111 & 1101  =  1101    13

         black king moved:     1111 & 0011  =  1011    3
  black king: rook moved:     1111 & 1011  =  1011    11
 black queen: rook moved:     1111 & 0111  =  0111    7

*/

int castling_rights[128] = {
    7, 15, 15, 15, 3, 15, 15, 11, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    15, 15, 15, 15, 15, 15, 15, 15, o, o, o, o, o, o, o, o,
    13, 15, 15, 15, 12, 15, 15, 14, o, o, o, o, o, o, o, o};

// positional scores
int positional_score[128] = {
    0, 0, 5, 0, -5, 0, 5, 0, o, o, o, o, o, o, o, o,
    5, 5, 0, 0, 0, 0, 5, 5, o, o, o, o, o, o, o, o,
    5, 10, 15, 20, 20, 15, 10, 5, o, o, o, o, o, o, o, o,
    5, 10, 20, 30, 30, 20, 10, 5, o, o, o, o, o, o, o, o,
    5, 10, 20, 30, 30, 20, 10, 5, o, o, o, o, o, o, o, o,
    5, 10, 15, 20, 20, 15, 10, 5, o, o, o, o, o, o, o, o,
    5, 5, 0, 0, 0, 0, 5, 5, o, o, o, o, o, o, o, o,
    0, 0, 5, 0, -5, 0, 5, 0, o, o, o, o, o, o, o, o};

// chess board representation
int board[128] = {
    r, n, b, q, k, b, n, r, o, o, o, o, o, o, o, o,
    p, p, p, p, p, p, p, p, o, o, o, o, o, o, o, o,
    e, e, e, e, e, e, e, e, o, o, o, o, o, o, o, o,
    e, e, e, e, e, e, e, e, o, o, o, o, o, o, o, o,
    e, e, e, e, e, e, e, e, o, o, o, o, o, o, o, o,
    e, e, e, e, e, e, e, e, o, o, o, o, o, o, o, o,
    P, P, P, P, P, P, P, P, o, o, o, o, o, o, o, o,
    R, N, B, Q, K, B, N, R, o, o, o, o, o, o, o, o};

// convert board square indexes to coordinates
char *square_to_coords[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "i8", "j8", "k8", "l8", "m8", "n8", "o8", "p8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "i7", "j7", "k7", "l7", "m7", "n7", "o7", "p7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "i6", "j6", "k6", "l6", "m6", "n6", "o6", "p6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "i5", "j5", "k5", "l5", "m5", "n5", "o5", "p5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "i4", "j4", "k4", "l4", "m4", "n4", "o4", "p4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "i3", "j3", "k3", "l3", "m3", "n3", "o3", "p3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2", "j2", "k2", "l2", "m2", "n2", "o2", "p2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", "j1", "k1", "l1", "m1", "n1", "o1", "p1"};

// decode promoted pieces
int promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

// side to move
int side = white;

// enpassant square
int enpassant = no_sq;

// castling rights (bin 1111)
int castle = 15;

// kings' squares
int king_square[2] = {e1, e8};

/*
    Move formatting

    0000 0000 0000 0000 0111 1111       source square
    0000 0000 0011 1111 1000 0000       target square
    0000 0011 1100 0000 0000 0000       promoted piece
    0000 0100 0000 0000 0000 0000       capture flag
    0000 1000 0000 0000 0000 0000       double pawn flag
    0001 0000 0000 0000 0000 0000       enpassant flag
    0010 0000 0000 0000 0000 0000       castling

*/

// encode move
#define encode_move(source, target, piece, capture, pawn, enpassant_, castling) \
    ((source) | (target << 7) | (piece << 14) | (capture << 18) | (pawn << 19) | (enpassant_ << 20) | (castling << 21))

// decode move: source square
#define get_move_source(move) (move & 0x7f)

// decode move: target square
#define get_move_target(move) ((move >> 7) & 0x7f)

// decode move: promoted piece
#define get_move_piece(move) ((move >> 14) & 0xf)

// decode move: capture flag
#define get_move_capture(move) ((move >> 18) & 0x1)

// decode move: double pawn push flag
#define get_move_pawn(move) ((move >> 19) & 0x1)

// decode move: enpassant flag
#define get_move_enpassant(move) ((move >> 20) & 0x1)

// decode move: castling flag
#define get_move_castling(move) ((move >> 21) & 0x1)

// piece move offsets
int knight_offsets[8] = {33, 31, 18, 14, -33, -31, -18, -14};
int bishop_offsets[4] = {15, 17, -15, -17};
int rook_offsets[4] = {16, -16, 1, -1};
int king_offsets[8] = {16, -16, 1, -1, 15, 17, -15, -17};

typedef struct
{
    int moves[256];
    int count;
} moves;

// Map bitboard (64) to 0x88 (128)
int bitboard_to_0x88[64] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    16, 17, 18, 19, 20, 21, 22, 23,
    32, 33, 34, 35, 36, 37, 38, 39,
    48, 49, 50, 51, 52, 53, 54, 55,
    64, 65, 66, 67, 68, 69, 70, 71,
    80, 81, 82, 83, 84, 85, 86, 87,
    96, 97, 98, 99, 100, 101, 102, 103,
    112, 113, 114, 115, 116, 117, 118, 119};

moves moves_from_bb(BitBoard bitboard)
{
    moves m;
    while (bitboard)
    {
        int bit_index = __builtin_ctzll(bitboard);
        int square_0x88 = bitboard_to_0x88[bit_index];
        m.moves[m.count] = square_0x88;
        m.count += 1;
        bitboard &= bitboard - 1;
    }
    return m;
}

BitBoard board_to_bitboard()
{
    BitBoard bb = 0ULL;
    for (int square = 0; square < 128; square++)
    {
        if ((square & 0x88) || board[square] != e)
            bb |= (1ULL << (square & 63));
    }

    return bb;
}

void reset_board()
{

    for (int rank = 0; rank < 8; rank++)
    {

        for (int file = 0; file < 16; file++)
        {
            int square = rank * 16 + file;

            if (!(square & 0x88))
                board[square] = e;
        }
    }

    side = -1;
    castle = 0;
    enpassant = no_sq;
}

void parse_fen(char *fen)
{
    // reset board
    reset_board();

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++)
    {
        // loop over board files
        for (int file = 0; file < 16; file++)
        {
            // init square
            int square = rank * 16 + file;

            // if square is on board
            if (!(square & 0x88))
            {
                // match pieces
                if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
                {
                    // set up kings' squares
                    if (*fen == 'K')
                        king_square[white] = square;

                    else if (*fen == 'k')
                        king_square[black] = square;

                    // set the piece on board
                    board[square] = char_pieces[(int)*fen];

                    // increment FEN pointer
                    fen++;
                }

                // match empty squares
                if (*fen >= '0' && *fen <= '9')
                {
                    // calculate offset
                    int offset = *fen - '0';

                    // decrement file on empty squares
                    if (!(board[square]))
                        file--;

                    // skip empty squares
                    file += offset;

                    // increment FEN pointer
                    fen++;
                }

                // match end of rank
                if (*fen == '/')
                    // increment FEN pointer
                    fen++;
            }
        }
    }

    // go to side parsing
    fen++;

    // parse side to move
    side = (*fen == 'w') ? white : black;

    // go to castling rights parsing
    fen += 2;

    // parse castling rights
    while (*fen != ' ')
    {
        switch (*fen)
        {
        case 'K':
            castle |= KC;
            break;
        case 'Q':
            castle |= QC;
            break;
        case 'k':
            castle |= kc;
            break;
        case 'q':
            castle |= qc;
            break;
        case '-':
            break;
        }

        // increment pointer
        fen++;
    }

    // got to empassant square
    fen++;

    // parse empassant square
    if (*fen != '-')
    {
        // parse enpassant square's file & rank
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');

        // set up enpassant square
        enpassant = rank * 16 + file;
    }

    else
        enpassant = no_sq;
}

int is_square_attacked(int square, int side)
{
    // pawn attacks
    if (!side)
    {
        // if target square is on board and is white pawn
        if (!((square + 17) & 0x88) && (board[square + 17] == P))
            return 1;

        // if target square is on board and is white pawn
        if (!((square + 15) & 0x88) && (board[square + 15] == P))
            return 1;
    }

    else
    {
        // if target square is on board and is black pawn
        if (!((square - 17) & 0x88) && (board[square - 17] == p))
            return 1;

        // if target square is on board and is black pawn
        if (!((square - 15) & 0x88) && (board[square - 15] == p))
            return 1;
    }

    // knight attacks
    for (int index = 0; index < 8; index++)
    {
        // init target square
        int target_square = square + knight_offsets[index];

        // lookup target piece
        int target_piece = board[target_square];

        // if target square is on board
        if (!(target_square & 0x88))
        {
            if (!side ? target_piece == N : target_piece == n)
                return 1;
        }
    }

    // king attacks
    for (int index = 0; index < 8; index++)
    {
        // init target square
        int target_square = square + king_offsets[index];

        // lookup target piece
        int target_piece = board[target_square];

        // if target square is on board
        if (!(target_square & 0x88))
        {
            // if target piece is either white or black king
            if (!side ? target_piece == K : target_piece == k)
                return 1;
        }
    }

    // bishop & queen attacks
    for (int index = 0; index < 4; index++)
    {
        // init target square
        int target_square = square + bishop_offsets[index];

        // loop over attack ray
        while (!(target_square & 0x88))
        {
            // target piece
            int target_piece = board[target_square];

            // if target piece is either white or black bishop or queen
            if (!side ? (target_piece == B || target_piece == Q) : (target_piece == b || target_piece == q))
                return 1;

            // break if hit a piece
            if (target_piece)
                break;

            // increment target square by move offset
            target_square += bishop_offsets[index];
        }
    }

    // rook & queen attacks
    for (int index = 0; index < 4; index++)
    {
        // init target square
        int target_square = square + rook_offsets[index];

        // loop over attack ray
        while (!(target_square & 0x88))
        {
            // target piece
            int target_piece = board[target_square];

            // if target piece is either white or black bishop or queen
            if (!side ? (target_piece == R || target_piece == Q) : (target_piece == r || target_piece == q))
                return 1;

            // break if hit a piece
            if (target_piece)
                break;

            // increment target square by move offset
            target_square += rook_offsets[index];
        }
    }

    return 0;
}

// populate move list
void add_move(moves *move_list, int move)
{
    // push move into the move list
    move_list->moves[move_list->count] = move;

    // increment move count
    move_list->count++;
}

// move generator
void generate_moves(moves *move_list)
{
    // reset move count
    move_list->count = 0;

    // loop over all board squares
    for (int square = 0; square < 128; square++)
    {
        // check if the square is on board
        if (!(square & 0x88))
        {
            // white pawn and castling moves
            if (!side)
            {
                // white pawn moves
                if (board[square] == P)
                {
                    // init target square
                    int to_square = square - 16;

                    // quite white pawn moves (check if target square is on board)
                    if (!(to_square & 0x88) && !board[to_square])
                    {
                        // pawn promotions
                        if (square >= a7 && square <= h7)
                        {
                            add_move(move_list, encode_move(square, to_square, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, N, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((square >= a2 && square <= h2) && !board[square - 32])
                                add_move(move_list, encode_move(square, (square - 32), 0, 0, 1, 0, 0));
                        }
                    }

                    // white pawn capture moves
                    for (int index = 0; index < 4; index++)
                    {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[index];

                        // white pawn offsets
                        if (pawn_offset < 0)
                        {
                            // init target square
                            int to_square = square + pawn_offset;

                            // check if target square is on board
                            if (!(to_square & 0x88))
                            {
                                // capture pawn promotion
                                if (
                                    (square >= a7 && square <= h7) &&
                                    (board[to_square] >= 7 && board[to_square] <= 12))
                                {
                                    add_move(move_list, encode_move(square, to_square, Q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, R, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, B, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, N, 1, 0, 0, 0));
                                }

                                else
                                {
                                    // casual capture
                                    if (board[to_square] >= 7 && board[to_square] <= 12)
                                        add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));

                                    // enpassant capture
                                    if (to_square == enpassant)
                                        add_move(move_list, encode_move(square, to_square, 0, 1, 0, 1, 0));
                                }
                            }
                        }
                    }
                }

                // white king castling
                if (board[square] == K)
                {
                    // if king side castling is available
                    if (castle & KC)
                    {
                        // make sure there are empty squares between king & rook
                        if (!board[f1] && !board[g1])
                        {
                            // make sure king & next square are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
                                add_move(move_list, encode_move(e1, g1, 0, 0, 0, 0, 1));
                        }
                    }

                    // if queen side castling is available
                    if (castle & QC)
                    {
                        // make sure there are empty squares between king & rook
                        if (!board[d1] && !board[b1] && !board[c1])
                        {
                            // make sure king & next square are not under attack
                            if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                                add_move(move_list, encode_move(e1, c1, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }

            // black pawn and castling moves
            else
            {
                // black pawn moves
                if (board[square] == p)
                {
                    // init target square
                    int to_square = square + 16;

                    // quite black pawn moves (check if target square is on board)
                    if (!(to_square & 0x88) && !board[to_square])
                    {
                        // pawn promotions
                        if (square >= a2 && square <= h2)
                        {
                            add_move(move_list, encode_move(square, to_square, q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, r, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, b, 0, 0, 0, 0));
                            add_move(move_list, encode_move(square, to_square, n, 0, 0, 0, 0));
                        }

                        else
                        {
                            // one square ahead pawn move
                            add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));

                            // two squares ahead pawn move
                            if ((square >= a7 && square <= h7) && !board[square + 32])
                                add_move(move_list, encode_move(square, (square + 32), 0, 0, 1, 0, 0));
                        }
                    }

                    // black pawn capture moves
                    for (int index = 0; index < 4; index++)
                    {
                        // init pawn offset
                        int pawn_offset = bishop_offsets[index];

                        // white pawn offsets
                        if (pawn_offset > 0)
                        {
                            // init target square
                            int to_square = square + pawn_offset;

                            // check if target square is on board
                            if (!(to_square & 0x88))
                            {
                                // capture pawn promotion
                                if (
                                    (square >= a2 && square <= h2) &&
                                    (board[to_square] >= 1 && board[to_square] <= 6))
                                {
                                    add_move(move_list, encode_move(square, to_square, q, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, r, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, b, 1, 0, 0, 0));
                                    add_move(move_list, encode_move(square, to_square, n, 1, 0, 0, 0));
                                }

                                else
                                {
                                    // casual capture
                                    if (board[to_square] >= 1 && board[to_square] <= 6)
                                        add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));

                                    // enpassant capture
                                    if (to_square == enpassant)
                                        add_move(move_list, encode_move(square, to_square, 0, 1, 0, 1, 0));
                                }
                            }
                        }
                    }
                }

                // black king castling
                if (board[square] == k)
                {
                    // if king side castling is available
                    if (castle & kc)
                    {
                        // make sure there are empty squares between king & rook
                        if (!board[f8] && !board[g8])
                        {
                            // make sure king & next square are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                                add_move(move_list, encode_move(e8, g8, 0, 0, 0, 0, 1));
                        }
                    }

                    // if queen side castling is available
                    if (castle & qc)
                    {
                        // make sure there are empty squares between king & rook
                        if (!board[d8] && !board[b8] && !board[c8])
                        {
                            // make sure king & next square are not under attack
                            if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                                add_move(move_list, encode_move(e8, c8, 0, 0, 0, 0, 1));
                        }
                    }
                }
            }

            // knight moves
            if (!side ? board[square] == N : board[square] == n)
            {
                // loop over knight move offsets
                for (int index = 0; index < 8; index++)
                {
                    // init target square
                    int to_square = square + knight_offsets[index];

                    // init target piece
                    int piece = board[to_square];

                    // make sure target square is onboard
                    if (!(to_square & 0x88))
                    {
                        //
                        if (
                            !side ? (!piece || (piece >= 7 && piece <= 12)) : (!piece || (piece >= 1 && piece <= 6)))
                        {
                            // on capture
                            if (piece)
                                add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));

                            // on empty square
                            else
                                add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));
                        }
                    }
                }
            }

            // king moves
            if (!side ? board[square] == K : board[square] == k)
            {
                // loop over king move offsets
                for (int index = 0; index < 8; index++)
                {
                    // init target square
                    int to_square = square + king_offsets[index];

                    // init target piece
                    int piece = board[to_square];

                    // make sure target square is onboard
                    if (!(to_square & 0x88))
                    {
                        //
                        if (
                            !side ? (!piece || (piece >= 7 && piece <= 12)) : (!piece || (piece >= 1 && piece <= 6)))
                        {
                            // on capture
                            if (piece)
                                add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));

                            // on empty square
                            else
                                add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));
                        }
                    }
                }
            }

            // bishop & queen moves
            if (
                !side ? (board[square] == B) || (board[square] == Q) : (board[square] == b) || (board[square] == q))
            {
                // loop over bishop & queen offsets
                for (int index = 0; index < 4; index++)
                {
                    // init target square
                    int to_square = square + bishop_offsets[index];

                    // loop over attack ray
                    while (!(to_square & 0x88))
                    {
                        // init target piece
                        int piece = board[to_square];

                        // if hits own piece
                        if (!side ? (piece >= 1 && piece <= 6) : ((piece >= 7 && piece <= 12)))
                            break;

                        // if hits opponent: piece
                        if (!side ? (piece >= 7 && piece <= 12) : ((piece >= 1 && piece <= 6)))
                        {
                            add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));
                            break;
                        }

                        // if steps into an empty squre
                        if (!piece)
                            add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));

                        // increment target square
                        to_square += bishop_offsets[index];
                    }
                }
            }

            // rook & queen moves
            if (
                !side ? (board[square] == R) || (board[square] == Q) : (board[square] == r) || (board[square] == q))
            {
                // loop over rook &queen offsets
                for (int index = 0; index < 4; index++)
                {
                    // init target square
                    int to_square = square + rook_offsets[index];

                    // loop over attack ray
                    while (!(to_square & 0x88))
                    {
                        // init target piece
                        int piece = board[to_square];

                        // if hits own piece
                        if (!side ? (piece >= 1 && piece <= 6) : ((piece >= 7 && piece <= 12)))
                            break;

                        // if hits opponent: piece
                        if (!side ? (piece >= 7 && piece <= 12) : ((piece >= 1 && piece <= 6)))
                        {
                            add_move(move_list, encode_move(square, to_square, 0, 1, 0, 0, 0));
                            break;
                        }

                        // if steps into an empty squre
                        if (!piece)
                            add_move(move_list, encode_move(square, to_square, 0, 0, 0, 0, 0));

                        // increment target square
                        to_square += rook_offsets[index];
                    }
                }
            }
        }
    }
}

// make move
int make_move(int move, int capture_flag)
{
    // quiet move
    if (capture_flag == all_moves)
    {
        // define board state variable copies
        int board_copy[128], king_square_copy[2];
        int side_copy, enpassant_copy, castle_copy;

        // copy board state
        memcpy(board_copy, board, 512);
        side_copy = side;
        enpassant_copy = enpassant;
        castle_copy = castle;
        memcpy(king_square_copy, king_square, 8);

        // parse move
        int from_square = get_move_source(move);
        int to_square = get_move_target(move);
        int promoted_piece = get_move_piece(move);
        int enpass = get_move_enpassant(move);
        int double_push = get_move_pawn(move);
        int castling = get_move_castling(move);

        // move piece
        board[to_square] = board[from_square];
        board[from_square] = e;

        // pawn promotion
        if (promoted_piece)
            board[to_square] = promoted_piece;

        // enpassant capture
        if (enpass)
            !side ? (board[to_square + 16] = e) : (board[to_square - 16] = e);

        // reset enpassant square
        enpassant = no_sq;

        // double pawn push
        if (double_push)
            !side ? (enpassant = to_square + 16) : (enpassant = to_square - 16);

        // castling
        if (castling)
        {
            // switch target square
            switch (to_square)
            {
            // white castles king side
            case g1:
                board[f1] = board[h1];
                board[h1] = e;
                break;

            // white castles queen side
            case c1:
                board[d1] = board[a1];
                board[a1] = e;
                break;

                // black castles king side
            case g8:
                board[f8] = board[h8];
                board[h8] = e;
                break;

                // black castles queen side
            case c8:
                board[d8] = board[a8];
                board[a8] = e;
                break;
            }
        }

        // update king square
        if (board[to_square] == K || board[to_square] == k)
            king_square[side] = to_square;

        // update castling rights
        castle &= castling_rights[from_square];
        castle &= castling_rights[to_square];

        // change side
        side ^= 1;

        // take move back if king is under the check
        if (is_square_attacked(!side ? king_square[side ^ 1] : king_square[side ^ 1], side))
        {
            // restore board position
            memcpy(board, board_copy, 512);
            side = side_copy;
            enpassant = enpassant_copy;
            castle = castle_copy;
            memcpy(king_square, king_square_copy, 8);

            // illegal move
            return 0;
        }

        else
            // legal move
            return 1;
    }
    // capture move
    else
    {
        // if move is a capture
        if (get_move_capture(move))
            // make capture move
            return make_move(move, all_moves);

        else
            // move is not a capture
            return 0;
    }
}

int evaluate_position()
{
    // init score
    int score = 0;

    // loop over board squares
    for (int square = 0; square < 128; square++)
    {
        // make sure square is on board
        if (!(square & 0x88))
        {
            // init piece
            int piece = board[square];

            // material evaluation
            score += material_score[piece];

            // piece is white
            if (piece >= 1 && piece <= 6)
            {
                score += positional_score[square];
            }

            // black pieces
            else if (piece >= 7 && piece <= 12)
            {
                score -= positional_score[square];
            }
        }
    }

    // return positive score for white & negative for black
    return !side ? score : -score;
}

long nodes = 0;

#define copy_board()                            \
    int board_copy[128], king_square_copy[2];   \
    int side_copy, enpassant_copy, castle_copy; \
    memcpy(board_copy, board, 512);             \
    side_copy = side;                           \
    enpassant_copy = enpassant;                 \
    castle_copy = castle;                       \
    memcpy(king_square_copy, king_square, 8);

#define take_back()                 \
    memcpy(board, board_copy, 512); \
    side = side_copy;               \
    enpassant = enpassant_copy;     \
    castle = castle_copy;           \
    memcpy(king_square, king_square_copy, 8);

// best move
int best_move = 0;

// half move counter
int ply = 0;

// negamax serach with alpha-beta pruning
int negamax(int alpha, int beta, int depth)
{
    // escape condition
    if (!depth)
        // evaluate position
        return evaluate_position();

    // increment nodes
    nodes++;

    // legal moves counter
    int legal_moves = 0;

    // init old alpha
    int old_alpha = alpha;

    // init best move so far
    int bestmove_sofar = 0;

    // create move list
    moves move_list[1];

    // generate moves
    generate_moves(move_list);

    // loop over move list
    for (int count = 0; count < move_list->count; count++)
    {
        // preserve board position
        copy_board();

        // increment ply
        ply++;

        // make only legal moves
        if (!make_move(move_list->moves[count], all_moves))
        {
            // decrement ply
            ply--;

            // skip to the next move
            continue;
        }

        // increment legal moves counter
        legal_moves++;

        // recursive negamax call
        int score = -negamax(-beta, -alpha, depth - 1);

        // decrement ply
        ply--;

        // take move back
        take_back();

        // fail hard beta cutoff
        if (score >= beta)
        {
            return beta;
        }

        // found a better move
        if (score > alpha)
        {
            // increase lower bound
            alpha = score;

            if (!ply)
                // associate best move so far with best score so far
                bestmove_sofar = move_list->moves[count];
        }
    }

    // associate best move with best score
    if (old_alpha != alpha)
        best_move = bestmove_sofar;

    // if no legal moves available in the position
    if (!legal_moves)
    {
        // checkmate case
        if (is_square_attacked(king_square[side], side ^ 1))
            return -49000 + ply;

        // stalemate case
        else
            return 0;
    }

    return alpha;
}

int search(int depth)
{
    // run negamax alpha beta search on a given depth
    int score = negamax(-50000, 50000, depth);
    return score;
}

// parse move (from UCI)
int parse_move(char *move_str)
{
    // init move list
    moves move_list[1];

    // generate moves
    generate_moves(move_list);

    // parse move string
    int parse_from = (move_str[0] - 'a') + (8 - (move_str[1] - '0')) * 16;
    int parse_to = (move_str[2] - 'a') + (8 - (move_str[3] - '0')) * 16;
    int prom_piece = 0;

    // init move to encode
    int move;

    // loop over generated moves
    for (int count = 0; count < move_list->count; count++)
    {
        // pick up move
        move = move_list->moves[count];

        // if input move is present in the move list
        if (get_move_source(move) == parse_from && get_move_target(move) == parse_to)
        {
            // init promoted piece
            prom_piece = get_move_piece(move);

            // if promoted piece is present compare it with promoted piece from user input
            if (prom_piece)
            {
                if ((prom_piece == N || prom_piece == n) && move_str[4] == 'n')
                    return move;

                else if ((prom_piece == B || prom_piece == b) && move_str[4] == 'b')
                    return move;

                else if ((prom_piece == R || prom_piece == r) && move_str[4] == 'r')
                    return move;

                else if ((prom_piece == Q || prom_piece == q) && move_str[4] == 'q')
                    return move;

                continue;
            }

            // return move to make on board
            return move;
        }
    }

    // return illegal move
    return 0;
}

// input buffer size
#define inputBuffer (400 * 6)

// UCI driver
void uci()
{
    // init input buffer
    char line[inputBuffer];

    // main loop
    while (1)
    {
        // reset input buffer
        memset(&line[0], 0, sizeof(line));

        // flush stdout
        fflush(stdout);

        // skip on no user input
        if (!fgets(line, inputBuffer, stdin))
            continue;

        // skip on empty user input
        if (line[0] == '\n')
            continue;

        // parse "ucinewgame" command
        else if (!strncmp(line, "ucinewgame", 10))
        {
            // init board with initial position
            parse_fen(start_position);
        }

        // parse "isready" command
        else if (!strncmp(line, "isready", 7))
        {
            // return engine is ready
            printf("readyok\n");
            continue;
        }

        // init board from FEN
        else if (!strncmp(line, "position fen", 12))
        {
            // parse FEN input
            char *fen = line;
            fen += 13;

            // init board with current FEN
            parse_fen(fen);
        }

        // parse depth "go" command
        else if (!strncmp(line, "go depth", 8))
        {
            // parse "go" command
            char *go = line;
            go += 9;

            // parse depth
            int depth = *go - '0';

            // serch position with carrent depth
            int score = search(depth);

            // output best move
            printf("%d %s %s %c\n", score, square_to_coords[get_move_source(best_move)],
                   square_to_coords[get_move_target(best_move)],
                   promoted_pieces[get_move_piece(best_move)]);
        }

        // use fixed depth 4
        else if (!strncmp(line, "go", 2))
        {
            // search position with current depth 4
            int score = search(5);

            // output best move
            printf("%d %s %s %c\n", score, square_to_coords[get_move_source(best_move)],
                   square_to_coords[get_move_target(best_move)],
                   promoted_pieces[get_move_piece(best_move)]);
        }

        // parse "quit" command
        else if (!strncmp(line, "quit", 4))
            break;
    }
}

void decode_move(int move)
{
    printf("source: %i\n", move & 127);
    printf("target: %i\n", (move & 16256) >> 7);
    printf("promoted: %i\n", (move & 245760) >> 14);
    printf("capture: %i\n", (move & 262144) >> 18);
    printf("double pawn: %i\n", (move & 524288) >> 19);
    printf("enpassant: %i\n", (move & 1048576) >> 20);
    printf("castling: %i\n", (move & 2097152) >> 21);
}

int main()
{
    load_magic();
    init_all_pieces();
    uci();

    // char tricky_position[] = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    // parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    // search(4);
    // printf("%i\n", best_move);
    // decode_move(best_move);

    return 0;
}