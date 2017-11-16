#include "head.h"

long long unsigned int nodes = 0;

int Minimax (line *pline, fighter *board, int *chessboard, int turn, int depth)
{
	line path;
	// terminal 
	if (board->w_king == 0 || board->b_king == 0)
	{
		pline->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0)
	{
		pline->pv_count = 0;
		return Evaluate(chessboard); // evaluating function
	}

	int m = -INF;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);
	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16  capture = DoMove(movelist[i], board, chessboard, turn);
		int t = -Minimax(&path, board, chessboard, 1 - turn, depth - 1);
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (t > m)
		{
			m = t;
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
	}
	
	return m;
}