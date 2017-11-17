#include "head.h"

bool Uchifuzume_Searching (Bitboard *board, int *chessboard, int turn, int depth)
{
	// terminal 
	if (board->w_king == 0 || board->b_king == 0)
		return false;

	if (depth == 0)
		return (board->w_king == 0 || board->b_king == 0) ? false : true;

	bool ret = false;
	int cnt = 0;
	U16 movelist[MAX_MOVE_NUM] = {BLANK};

	cnt = (depth == DEPTH_UCHI) ? \
		UchifuzumeGenerator(*board, movelist, turn, PUT_PAWN_BACK) :\
		UchifuzumeGenerator(*board, movelist, turn, NULL);

	for (int i = 0; i < cnt; i++)
	{
		U16  capture = DoMove(movelist[i], board, chessboard, turn);
		bool t = !Uchifuzume_Searching(board, chessboard, 1 - turn, depth - 1);
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (t) return t; // cut off
	}

	return ret;
}