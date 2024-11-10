#ifndef _PREPARE_H_
#define _PREPARE_H_

#include "pieces.h"
#include "board.h"
#include <stdlib.h>

typedef Move_sqares **Prepared_moves;

typedef struct move_table
{
    Move_sqares w_pawn;
    Move_sqares w_pawn_attack;
    Move_sqares b_pawn;
    Move_sqares b_pawn_attack;
    Move_sqares knight;
    Move_sqares bishop;
    Move_sqares rook;
    Move_sqares queen;
    Move_sqares king;
} move_table;

#endif