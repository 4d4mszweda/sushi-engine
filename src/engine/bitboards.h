#ifndef _MAGICBOARDS_H_
#define _MAGICBOARDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

// functions
void init_all_pieces();
BitBoard get_bishop_attacks(int square, BitBoard occupancy);
BitBoard get_rook_attacks(int square, BitBoard occupancy);
void load_magic();

// bits manipulations
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)

// magic tables
BitBoard magic_table_rook[64];
BitBoard magic_table_bishop[64];

// masks
BitBoard bishop_masks[64];
BitBoard rook_masks[64];

// Files to exclude
BitBoard not_a_file = 18374403900871474942ULL;
BitBoard not_h_file = 9187201950435737471ULL;
BitBoard not_hg_file = 4557430888798830399ULL;
BitBoard not_ab_file = 18229723555195321596ULL;

// Rellevant bits
int rook_rellevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12};

int bishop_rellevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6};

#endif