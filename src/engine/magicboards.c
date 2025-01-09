#include "bitboards.h"

// random number
unsigned int state = 1804289383;

// 32-bit number pseudo random generator
unsigned int generate_random_number()
{
    // XOR shift algorithm
    unsigned int x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

BitBoard random_BitBoard()
{

    BitBoard u1, u2, u3, u4;

    u1 = (BitBoard)(generate_random_number()) & 0xFFFF;
    u2 = (BitBoard)(generate_random_number()) & 0xFFFF;
    u3 = (BitBoard)(generate_random_number()) & 0xFFFF;
    u4 = (BitBoard)(generate_random_number()) & 0xFFFF;

    // shuffle bits and return
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

BitBoard random_fewbits()
{
    return random_BitBoard() & random_BitBoard() & random_BitBoard();
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

// mask rook attacks
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

// find magic number
BitBoard find_magic(int square, int relevant_bits, int bishop)
{
    // define occupancies array
    BitBoard occupancies[4096];

    // define attacks array
    BitBoard attacks[4096];

    // define used indicies array
    BitBoard used_attacks[4096];

    // mask piece attack
    BitBoard mask_attack = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    // occupancy variations
    int occupancy_variations = 1 << relevant_bits;

    // loop over the number of occupancy variations
    for (int count = 0; count < occupancy_variations; count++)
    {
        // init occupancies
        occupancies[count] = set_occupancy(count, relevant_bits, mask_attack);

        // init attacks
        attacks[count] = bishop ? bishop_attacks_on_the_fly(square, occupancies[count]) : rook_attacks_on_the_fly(square, occupancies[count]);
    }

    // test magic numbers
    for (int random_count = 0; random_count < 100000000; random_count++)
    {
        // init magic number candidate
        BitBoard magic = random_fewbits();

        // skip testing magic number if inappropriate
        if (count_bits((mask_attack * magic) & 0xFF00000000000000ULL) < 6)
            continue;

        // reset used attacks array
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init count & fail flag
        int count, fail;

        // test magic index
        for (count = 0, fail = 0; !fail && count < occupancy_variations; count++)
        {
            // generate magic index
            int magic_index = (int)((occupancies[count] * magic) >> (64 - relevant_bits));

            // if got free index
            if (used_attacks[magic_index] == 0ULL)
                // assign corresponding attack map
                used_attacks[magic_index] = attacks[count];

            // otherwise fail on  collision
            else if (used_attacks[magic_index] != attacks[count])
                fail = 1;
        }

        // return magic if it works
        if (!fail)
            return magic;
    }

    // on fail
    printf("***Failed***\n");
    return 0ULL;
}

// init magics
void save_magic()
{

    BitBoard rook_magic_number[64], bishop_magic_number[64];
    for (int square = 0; square < 64; square++)
    {
        rook_magic_number[square] = find_magic(square, rook_rellevant_bits[square], rook);
        bishop_magic_number[square] = find_magic(square, rook_rellevant_bits[square], bishop);
    }

    FILE *f1 = fopen("src/engine/rook_magic", "wb");
    FILE *f2 = fopen("src/engine/bishop_magic", "wb");

    fwrite(rook_magic_number, sizeof(BitBoard), 64, f1);
    fwrite(bishop_magic_number, sizeof(BitBoard), 64, f2);

    fclose(f1);
    fclose(f2);
}

int main()
{
    save_magic();
}
