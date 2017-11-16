#include "head.h"

//U64 max_move = 0;

int NegaMax (line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth)
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
		//return (LIMIT_DEPTH & 1) ? -Evaluate(chessboard) : Evaluate(chessboard); // evaluating function
		return QuiescenceSearch(board, chessboard, alpha, beta, turn);
	}
	
	int bestscore = -INF; // soft initial value
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);
    ++nodes;
    total_branch += cnt;
    //if (cnt > max_move)
    //    printf("%d ", (max_move = cnt));

	for (int i = 0; i < cnt; i++)
	{
        ++search_branch;
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = -NegaMax(&path, board, chessboard, -beta, -max(alpha, bestscore), 1 - turn, depth - 1);
		if (score > bestscore)
		{
			bestscore = score;
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
	}
	return bestscore;
}

int AlphaBetaMax(line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (board->w_king == 0)
	{
		pline->pv_count = 0;
		return -CHECKMATE;
	}
	if (depth == 0)
	{
		pline->pv_count = 0;
		return Evaluate(chessboard); // evaluating function
	}

	int bestscore = -INF;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = AlphaBetaMin(&path, board, chessboard, max(alpha, bestscore), beta, 1 - turn, depth - 1);
		if (score > bestscore)
		{
			bestscore = score;
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
	}

	return bestscore;
}

int AlphaBetaMin(line *pline, fighter *board, int *chessboard, int alpha, int beta, int turn, int depth)
{
	line path;
	// terminal 
	if (board->b_king == 0)
	{
		pline->pv_count = 0;
		return CHECKMATE;
	}
	if (depth == 0)
	{
		pline->pv_count = 0;
		return -Evaluate(chessboard); // evaluating function
	}

	int bestscore = INF;
	U16 movelist[MAX_MOVE_NUM] = {0};
	int cnt = Generator(*board, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		nodes++;
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = AlphaBetaMax(&path, board, chessboard, alpha, min(beta, bestscore), 1 - turn, depth - 1);
		if (score < bestscore)
		{
			bestscore = score;
			pline->pv[0] = movelist[i];
			memcpy(pline->pv + 1, path.pv, path.pv_count * sizeof(U16));
			pline->pv_count = path.pv_count + 1;
		}
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore <= alpha) return bestscore; // cut off
	}

	return bestscore;
}