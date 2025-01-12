#ifndef _BITBOARDS_H_
#define _BITBOARDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BitBoard unsigned long long

// functions
void init_all_pieces();
BitBoard get_bishop_attacks(int square, BitBoard occupancy);
BitBoard get_rook_attacks(int square, BitBoard occupancy);
void load_magic();
int count_bits(BitBoard bitboard);
int get_ls1b_index(BitBoard bitboard);
BitBoard set_occupancy(int index, int rellevant_bits, BitBoard attack_mask);
BitBoard mask_bishop_attacks(int square);
BitBoard mask_rook_attacks(int square);
BitBoard bishop_attacks_on_the_fly(int square, BitBoard block);
BitBoard rook_attacks_on_the_fly(int square, BitBoard block);
void save_magic();

// bits manipulations
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

// magic tables
extern BitBoard magic_table_rook[64];
extern BitBoard magic_table_bishop[64];

// masks
extern BitBoard bishop_masks[64];
extern BitBoard rook_masks[64];

// Files to exclude
extern BitBoard not_a_file;
extern BitBoard not_h_file;
extern BitBoard not_hg_file;
extern BitBoard not_ab_file;

// Rellevant bits
extern int rook_rellevant_bits[64];
extern int bishop_rellevant_bits[64];

// Attacks
extern BitBoard bishop_attacks[64][512];
extern BitBoard rook_attacks[64][4096];
extern BitBoard pawn_attacks[2][64];
extern BitBoard knight_attacks[64];
extern BitBoard king_attacks[64];

// colors

enum colors
{
    white,
    black
};

#endif