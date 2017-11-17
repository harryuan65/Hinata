#include "head.h"
#include "move.h"

void Inital (Bitboard *board, int *chessboard)
{
	board->w_occupied = WHITE_INIT;
	board->b_occupied = BLACK_INIT;

	board->w_pawn     = W_PAWN_INIT;
	board->w_silver   = W_SILVER_INIT;
	board->w_gold     = W_GOLD_INIT;
	board->w_bishop   = W_BISHOP_INIT;
	board->w_rook     = W_ROOK_INIT;
	board->w_king     = W_KING_INIT;
	board->w_e_pawn   = 0;
	board->w_e_silver = 0;
	board->w_e_bishop = 0;
	board->w_e_rook   = 0;

	board->b_pawn   = B_PAWN_INIT;
	board->b_silver = B_SILVER_INIT;
	board->b_gold   = B_GOLD_INIT;
	board->b_bishop = B_BISHOP_INIT;
	board->b_rook   = B_ROOK_INIT;
	board->b_king   = B_KING_INIT;
	board->b_e_pawn   = 0;
	board->b_e_silver = 0;
	board->b_e_bishop = 0;
	board->b_e_rook   = 0;

	memset (chessboard, BLANK, CHESS_BOARD_SIZE * sizeof(int));

	chessboard[A5] = ROOK   | COLOR_BOUND;
	chessboard[A4] = BISHOP | COLOR_BOUND;
	chessboard[A3] = SILVER | COLOR_BOUND;
	chessboard[A2] = GOLD   | COLOR_BOUND;
	chessboard[A1] = KING   | COLOR_BOUND;
	chessboard[B1] = PAWN   | COLOR_BOUND;

	chessboard[E1] = ROOK;
	chessboard[E2] = BISHOP;
	chessboard[E3] = SILVER;
	chessboard[E4] = GOLD;
	chessboard[E5] = KING;
	chessboard[D5] = PAWN;

	return ;
}