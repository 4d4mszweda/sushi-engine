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

Prepared_moves **prepare_knight()
{
    Position starting_square = 0ULL;
    Move_sqares ms = 0ULL;
    Prepared_moves pm = malloc_pm();

    int knight_moves[][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    for (int row = 0; row < B_MAX_ROW; row++)
    {
        for (int column = 0; column < B_MAX_COL; column++)
        {
            for (int km; km < 8; km++)
            {
                int move_row = row + knight_moves[row][column];
                int move_col = column + knight_moves[row][column];

                if (move_row >= 0 && move_row < 8 &&
                    move_col >= 0 && move_col < 8)
                {
                    ms |= 1ULL << ((move_row << 3) + move_col);
                }
            }
            pm[row][column] = ms;
            starting_square++;
            ms = 0ULL;
        }
    }
    return pm;
}