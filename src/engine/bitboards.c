#include "bitboards.h"

BitBoard mask_pawn_attacks(int side, int square)
{
    // attack bitboard
    BitBoard attacks = 0;

    // piece bitboard
    BitBoard bitboard = 0;

    // set piece on bitboard
    set_bit(bitboard, square);

    // white pawn attacks
    if (!side)
    {
        // make sure attack is on board
        if ((bitboard >> 7) & not_a_file)
            attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file)
            attacks |= (bitboard >> 9);
    }

    // black pawn atacks
    else
    {
        // make sure attack is on board
        if ((bitboard << 7) & not_h_file)
            attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file)
            attacks |= (bitboard << 9);
    }

    // return attack map for pawn on a given square
    return attacks;
}

// count bits (Kernighan's algorithm)
int count_bits(BitBoard bitboard)
{

    int count = 0;

    while (bitboard)
    {
        count++;
        bitboard &= bitboard - 1;
    }

    return count;
}

// get index of LS1B in bitboard
int get_ls1b_index(BitBoard bitboard)
{
    if (bitboard != 0)
        return count_bits((bitboard & -bitboard) - 1);
    else
        return -1;
}

// mask knight attacks
BitBoard mask_knight_attacks(int square)
{
    // attack bitboard
    BitBoard attacks = 0;

    // piece bitboard
    BitBoard bitboard = 0;

    // set piece on bitboard
    set_bit(bitboard, square);

    // generate knight
    if ((bitboard >> 17) & not_h_file)
        attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file)
        attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file)
        attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file)
        attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file)
        attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file)
        attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file)
        attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file)
        attacks |= (bitboard << 6);

    // return attack map for knight on a given square
    return attacks;
}

// mask king attacks
BitBoard mask_king_attacks(int square)
{
    // attack bitboard
    BitBoard attacks = 0;

    // piece bitboard
    BitBoard bitboard = 0;

    // set piece on bitboard
    set_bit(bitboard, square);

    // generate king attacks
    if (bitboard >> 8)
        attacks |= (bitboard >> 8);
    if (bitboard << 8)
        attacks |= (bitboard << 8);
    if ((bitboard >> 1) & not_h_file)
        attacks |= (bitboard >> 1);
    if ((bitboard >> 9) & not_h_file)
        attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file)
        attacks |= (bitboard >> 7);
    if ((bitboard << 1) & not_a_file)
        attacks |= (bitboard << 1);
    if ((bitboard << 9) & not_a_file)
        attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file)
        attacks |= (bitboard << 7);

    // return attack map for king on a given square
    return attacks;
}

BitBoard set_occupancy(int index, int bits_in_mask, BitBoard attack_mask)
{

    BitBoard occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++)
    {
        // get LS1B index of attacks mask
        int square = get_ls1b_index(attack_mask);

        // pop LS1B in attack map
        pop_bit(attack_mask, square);

        // make sure occupancy is on board
        if (index & (1 << count))
            // populate occupancy map
            occupancy |= (1ULL << square);
    }

    // return occupancy map
    return occupancy;
}

BitBoard mask_rook_attacks(int square)
{
    // attacks bitboard
    BitBoard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1; r <= 6; r++)
        attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--)
        attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++)
        attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--)
        attacks |= (1ULL << (tr * 8 + f));

    // return attack map for bishop on a given square
    return attacks;
}

// mask bishop attacks
BitBoard mask_bishop_attacks(int square)
{
    // attack bitboard
    BitBoard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++)
        attacks |= (1ULL << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--)
        attacks |= (1ULL << (r * 8 + f));

    // return attack map for bishop on a given square
    return attacks;
}

// bishop attacks
BitBoard bishop_attacks_on_the_fly(int square, BitBoard block)
{
    // attack bitboard
    BitBoard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--)
    {
        attacks |= (1ULL << (r * 8 + f));
        if (block & (1ULL << (r * 8 + f)))
            break;
    }

    // return attack map for bishop on a given square
    return attacks;
}

// rook attacks
BitBoard rook_attacks_on_the_fly(int square, BitBoard block)
{
    // attacks bitboard
    BitBoard attacks = 0ULL;

    // init files & ranks
    int f, r;

    // init target files & ranks
    int tr = square / 8;
    int tf = square % 8;

    // generate attacks
    for (r = tr + 1; r <= 7; r++)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf)))
            break;
    }

    for (r = tr - 1; r >= 0; r--)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if (block & (1ULL << (r * 8 + tf)))
            break;
    }

    for (f = tf + 1; f <= 7; f++)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f)))
            break;
    }

    for (f = tf - 1; f >= 0; f--)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if (block & (1ULL << (tr * 8 + f)))
            break;
    }

    // return attack map for bishop on a given square
    return attacks;
}

void init_sliders_attacks(int is_bishop)
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init bishop & rook masks
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        // init current mask
        BitBoard mask = is_bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

        // count attack mask bits
        int bit_count = count_bits(mask);

        // occupancy variations count
        int occupancy_variations = 1 << bit_count;

        // loop over occupancy variations
        for (int count = 0; count < occupancy_variations; count++)
        {
            // bishop
            if (is_bishop)
            {
                // init occupancies, magic index & attacks
                BitBoard occupancy = set_occupancy(count, bit_count, mask);
                BitBoard magic_index = occupancy * magic_table_bishop[square] >> (64 - bishop_rellevant_bits[square]);
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }

            // rook
            else
            {
                // init occupancies, magic index & attacks
                BitBoard occupancy = set_occupancy(count, bit_count, mask);
                BitBoard magic_index = occupancy * magic_table_rook[square] >> (64 - rook_rellevant_bits[square]);
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

// init pre-calculated attack tables for leaper pieces (pawns, knights, kings)
void init_leaper_attacks()
{
    for (int square = 0; square < 64; square++)
    {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}

void init_all_pieces()
{
    init_leaper_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);
}

BitBoard get_bishop_attacks(int square, BitBoard occupancy)
{

    occupancy &= bishop_masks[square];
    occupancy *= magic_table_bishop[square];
    occupancy >>= 64 - bishop_rellevant_bits[square];

    return bishop_attacks[square][occupancy];
}

BitBoard get_rook_attacks(int square, BitBoard occupancy)
{

    occupancy &= rook_masks[square];
    occupancy *= magic_table_rook[square];
    occupancy >>= 64 - rook_rellevant_bits[square];

    return rook_attacks[square][occupancy];
}

void load_magic()
{
    FILE *f1 = fopen("src/engine/rook_magic", "rb");
    fread(magic_table_rook, sizeof(BitBoard), 64, f1);
    FILE *f2 = fopen("src/engine/bishop_magic", "rb");
    fread(magic_table_bishop, sizeof(BitBoard), 64, f2);
};