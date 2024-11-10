#include "prepare.h"

Prepared_moves malloc_pm()
{
    Prepared_moves pm = malloc(B_MAX_ROW * sizeof(Move_sqares *));
    for (int i = 0; i < B_MAX_COL; i++)
        pm[i] = malloc(B_MAX_COL * sizeof(Move_sqares));
    return pm;
}

void free_pm(Prepared_moves pm)
{
    for (int i = 0; i < B_MAX_COL; i++)
        free(pm[i]);
    free(pm);
}

Move_sqares prepare_knight(int row, int col)
{
    Move_sqares ms = 0ULL;

    int knight_moves[][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    for (int km; km < 8; km++)
    {
        int move_row = row + knight_moves[row][col];
        int move_col = col + knight_moves[row][col];

        if (move_row >= 0 && move_row < 8 &&
            move_col >= 0 && move_col < 8)
        {
            ms |= 1ULL << ((move_row << 3) + move_col);
        }
    }
    ms = 0ULL;
    return ms;
}

Prepared_moves prepare_w_pawn(int row, int col)
{
    Move_sqares ms = 1ULL;
    return row == 1 ? ms << ((row << 3) + col) : ms << ((row << 4) + col);
}

Prepared_moves prepare_w_pawn_attack(int row, int col)
{
    Move_sqares ms = 0ULL;
}

Prepared_moves prepare_b_pawn(int row, int col)
{
    Move_sqares ms = 0ULL;
}

Prepared_moves prepare_b_pawn_attack(int row, int col)
{
    Move_sqares ms = 0ULL;
}

Prepared_moves prepare_king(int row, int col)
{
    Move_sqares ms = 0ULL;
}

move_table *generate_moves()
{
    move_table *mt;
    for (int row = 0; row < B_MAX_ROW; row++)
    {
        for (int column = 0; column < B_MAX_COL; column++)
        {
        }
    }
    return mt;
}