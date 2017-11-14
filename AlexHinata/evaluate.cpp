#include "evaluate.h"

/*
   The perspective of Black player.
 */
int Evaluate(int *chessboard) {
    int ret = 0, i = 0;
    for (; i < 25; ++i)
        if (chessboard[i])
            if (chessboard[i] & 16)
                ret -= pce_score[chessboard[i] & 15];
            else
                ret += pce_score[chessboard[i] & 15];

    for (;i < 45; ++i)
        if (chessboard[i])
            if (chessboard[i] & 16)
                ret -= hnd_score[chessboard[i] & 7];
            else
                ret += hnd_score[chessboard[i] & 7];

    return ret;
}