#include "head.h"

int QuiescenceSearch (Bitboard *board, int *chessboard, int alpha, int beta, int turn, bool isFailHigh) {
	nodes++;
	quiet_nodes++;
	failed_nodes += isFailHigh;
	// terminal 
	if (board->w_king == 0 || board->b_king == 0) {
		//cout << "Terminal CheckMate" << endl;
		//cout << nodes << endl;
		//PrintChessBoard(chessboard);
		if (!isFailHigh)
			leave_nodes++;
		return -CHECKMATE;
	}

	int bestscore = (turn == WHITE) ? Evaluate(chessboard) : -Evaluate(chessboard);
	if (bestscore >= beta) {
		//cout << "Terminal Beta" << endl;
		//PrintChessBoard(chessboard);
		if (!isFailHigh)
			leave_nodes++;
		return bestscore;
	}
	
	int cnt = 0;
	int n = beta;
	U16 movelist[MAX_MOVE_NUM] = {BLANK};
	AttackGenerator(&cnt, *board, movelist, turn);

	for (int i = 0; i < cnt; i++)
	{
		U16 capture = DoMove(movelist[i], board, chessboard, turn);
		int score = -QuiescenceSearch(board, chessboard, -n, -max(alpha, bestscore), 1 - turn, isFailHigh);
		
		if (score > bestscore) 
			bestscore = ((n == beta) || (score >= beta)) ? score : \
				-QuiescenceSearch(board, chessboard, -beta, -score + 1, 1 - turn, true);
		
		UnDoMove(capture, movelist[i], board, chessboard, turn);
		if (bestscore >= beta) return bestscore; // cut off
		n = max(alpha, bestscore) + 1; // set up a null window
	}
	return bestscore;
}