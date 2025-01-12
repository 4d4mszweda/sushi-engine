#include "../engine/bitboards.h"

void print_bitboard(BitBoard bitboard)
{
    printf("\n");

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;

            if (!file)
                printf("  %d ", 8 - rank);

            printf(" %d", get_bit(bitboard, square) ? 1 : 0);
        }

        printf("\n");
    }

    printf("\n     a b c d e f g h\n\n");

    printf("     bitboard: %llud\n\n", bitboard);
}

int main()
{
    // init magics
    load_magics();

    // init attack tables
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    // create bishop occupancy bitboard
    BitBoard bishop_occupancy = 0ULL;
    set_bit(bishop_occupancy, g7);
    set_bit(bishop_occupancy, f6);
    set_bit(bishop_occupancy, c5);
    set_bit(bishop_occupancy, b2);
    set_bit(bishop_occupancy, g1);

    printf("\n     Bishop occupancy\n");

    // print bishop occupancy
    print_bitboard(bishop_occupancy);

    printf("\n     Bishop attacks\n");

    // get bishop attacks
    print_bitboard(get_bishop_attacks(d4, bishop_occupancy));

    // create rook occupancy
    BitBoard rook_occupancy = 0ULL;
    set_bit(rook_occupancy, d7);
    set_bit(rook_occupancy, d6);
    set_bit(rook_occupancy, d3);
    set_bit(rook_occupancy, a4);
    set_bit(rook_occupancy, f4);

    printf("\n     Rook occupancy\n");

    // print rook occupancy
    print_bitboard(rook_occupancy);

    // get rook attacks
    print_bitboard(get_rook_attacks(d4, rook_occupancy));

    return 0;
}
